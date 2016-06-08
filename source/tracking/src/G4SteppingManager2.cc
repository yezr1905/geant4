// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4SteppingManager2.cc,v 1.1 2001/02/08 07:56:40 tsasaki Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
//
//---------------------------------------------------------------
//
// G4SteppingManager.icc
//
// Description:
//   This class represents the manager who steers to move the give
//   particle from the TrackingManger by one Step.
//
// Contact:
//   Questions and comments to this code should be sent to
//     Katsuya Amako  (e-mail: Katsuya.Amako@kek.jp)
//     Takashi Sasaki (e-mail: Takashi.Sasaki@kek.jp)
//
//---------------------------------------------------------------

#include "G4UImanager.hh"
#include "G4ForceCondition.hh"
#include "G4GPILSelection.hh"
#include "G4SteppingControl.hh"
#include "G4TransportationManager.hh"
#include "G4UserLimits.hh"
#include "G4EnergyLossTables.hh"
#include "G4SteppingManager.hh"

/////////////////////////////////////////////////
void G4SteppingManager::GetProcessNumber()
/////////////////////////////////////////////////
{

  G4ProcessManager* pm= fTrack->GetDefinition()->GetProcessManager();

// AtRestDoits
   MAXofAtRestLoops =        pm->GetAtRestProcessVector()->entries();
   fAtRestDoItVector =       pm->GetAtRestProcessVector(typeDoIt);
   fAtRestGetPhysIntVector = pm->GetAtRestProcessVector(typeGPIL);

// AlongStepDoits
   MAXofAlongStepLoops = pm->GetAlongStepProcessVector()->entries();
   fAlongStepDoItVector = pm->GetAlongStepProcessVector(typeDoIt);
   fAlongStepGetPhysIntVector = pm->GetAlongStepProcessVector(typeGPIL);

// PostStepDoits
   MAXofPostStepLoops = pm->GetPostStepProcessVector()->entries();
   fPostStepDoItVector = pm->GetPostStepProcessVector(typeDoIt);
   fPostStepGetPhysIntVector = pm->GetPostStepProcessVector(typeGPIL);
}


// ************************************************************************
//
//  Private Member Functions
//
// ************************************************************************


/////////////////////////////////////////////////////////
 void G4SteppingManager::DefinePhysicalStepLength()
/////////////////////////////////////////////////////////
{

// ReSet the counter etc.
   PhysicalStep  = DBL_MAX;          // Initialize by a huge number    
   physIntLength = DBL_MAX;          // Initialize by a huge number    
#ifdef G4VERBOSE
                         // !!!!! Verbose
           if(verboseLevel>0) fVerbose->DPSLStarted();
#endif

// Obtain the user defined maximum allowed Step in the volume
//   1997.12.13 adds argument for  GetMaxAllowedStep by K.Kurashige
   G4UserLimits* ul= fCurrentVolume->GetLogicalVolume()->GetUserLimits();
   if (ul) {
      physIntLength = ul->GetMaxAllowedStep(*fTrack);
#ifdef G4VERBOSE
                         // !!!!! Verbose
           if(verboseLevel>0) fVerbose->DPSLUserLimit();
#endif
   }

   if(physIntLength < PhysicalStep ){
      PhysicalStep = physIntLength;
      fStepStatus = fUserDefinedLimit;
      fStep->GetPostStepPoint()
           ->SetProcessDefinedStep(NULL);
      // Take note that the process pointer is 'NULL' if the Step
      // is defined by the user defined limit.
   }

// GPIL for PostStep
   fPostStepDoItProcTriggered = MAXofPostStepLoops;

   for(size_t np=0; np < MAXofPostStepLoops; np++){
     fCurrentProcess = (*fPostStepGetPhysIntVector)(np);
     if (fCurrentProcess== NULL) {
       (*fSelectedPostStepDoItVector)[np] = 0;
       continue;
     }   // NULL means the process is inactivated by a user on fly.

     physIntLength = fCurrentProcess->
                     PostStepGPIL( *fTrack,
                                                 fPreviousStepSize,
                                                      &fCondition );
#ifdef G4VERBOSE
                         // !!!!! Verbose
           if(verboseLevel>0) fVerbose->DPSLPostStep();
#endif

     switch (fCondition) {
     case ExclusivelyForced:
       (*fSelectedPostStepDoItVector)[np] = 4;
       fStepStatus = fExclusivelyForcedProc;
	   fStep->GetPostStepPoint()
				 ->SetProcessDefinedStep(fCurrentProcess);
       break;
     case Conditionally:
       (*fSelectedPostStepDoItVector)[np] = 3;
       break;
     case Forced:
       (*fSelectedPostStepDoItVector)[np] = 2;
       break;
     default:
       (*fSelectedPostStepDoItVector)[np] = 0;
       if(physIntLength < PhysicalStep ){
         PhysicalStep = physIntLength;
	 fStepStatus = fPostStepDoItProc;
	 fPostStepDoItProcTriggered = G4int(np);
         fStep->GetPostStepPoint()
              ->SetProcessDefinedStep(fCurrentProcess);
       }
     }
     if (fCondition==ExclusivelyForced) return;  // Take note the 'return' at here !!!
   }

   if(fPostStepDoItProcTriggered<MAXofPostStepLoops)
     (*fSelectedPostStepDoItVector)[fPostStepDoItProcTriggered] = 1;


// GPIL for AlongStep
   proposedSafety = DBL_MAX;
   G4double safetyProposedToAndByProcess = proposedSafety;

   for(size_t kp=0; kp < MAXofAlongStepLoops; kp++){
     fCurrentProcess = (*fAlongStepGetPhysIntVector)[kp];
     if (fCurrentProcess== NULL) continue;
         // NULL means the process is inactivated by a user on fly.

     physIntLength = fCurrentProcess->
                     AlongStepGPIL( *fTrack,
                                                  fPreviousStepSize,
                                                       PhysicalStep,
				       safetyProposedToAndByProcess,
                                                    &fGPILSelection );
#ifdef G4VERBOSE
                         // !!!!! Verbose
           if(verboseLevel>0) fVerbose->DPSLAlongStep();
#endif
     if(physIntLength < PhysicalStep){
       PhysicalStep = physIntLength;

       // Check if the process wants to be the GPIL winner. For example,
       // multi-scattering proposes Step limit, but won't be the winner.
       if(fGPILSelection==CandidateForSelection){
          fStepStatus = fAlongStepDoItProc;
          fStep->GetPostStepPoint()
               ->SetProcessDefinedStep(fCurrentProcess);
       }

    	  // Transportation is assumed to be the last process in the vector
       if(kp == MAXofAlongStepLoops-1) fStepStatus = fGeomBoundary;
     }

     // Make sure to check the safety, even if Step is not limited 
     //  by this process.                      J. Apostolakis, June 20, 1998
     // 
     if (safetyProposedToAndByProcess < proposedSafety)
        // proposedSafety keeps the smallest value:
        proposedSafety               = safetyProposedToAndByProcess;
     else
        // safetyProposedToAndByProcess always proposes a valid safety:
        safetyProposedToAndByProcess = proposedSafety;
      
   } 
} // void G4SteppingManager::DefinePhysicalStepLength() //


//////////////////////////////////////////////////////
void G4SteppingManager::InvokeAtRestDoItProcs()
//////////////////////////////////////////////////////
{

// Select the rest process which has the shortest time before
// it is invoked. In rest processes, GPIL()
// returns the time before a process occurs.
   G4double lifeTime, shortestLifeTime;

   fN2ndariesAtRestDoIt = 0;
   shortestLifeTime = DBL_MAX;

   int NofInactiveProc=0;
   for( size_t ri=0 ; ri < MAXofAtRestLoops ; ri++ ){
     fCurrentProcess = (*fAtRestGetPhysIntVector)[ri];
     if (fCurrentProcess== NULL) {
       (*fSelectedAtRestDoItVector)[ri] = 0;
       NofInactiveProc++;
       continue;
     }   // NULL means the process is inactivated by a user on fly.

     lifeTime = 
       fCurrentProcess->AtRestGPIL( 
                                                     *fTrack,
                                                &fCondition );
     if(fCondition==Forced){
       (*fSelectedAtRestDoItVector)[ri] = 2;
     }
     else{
       (*fSelectedAtRestDoItVector)[ri] = 0;
       if(lifeTime < shortestLifeTime ){
         shortestLifeTime = lifeTime;
	 fAtRestDoItProcTriggered =  G4int(int(ri)); 
       }
     }
   }

// at least one process is necessary to destory the particle  
// exit with warning 
   if(NofInactiveProc==MAXofAtRestLoops){ 
     G4Exception("G4SteppingManager::InvokeAtRestDoItProcs: No AtRestDoIt process is active. " );
   }

   (*fSelectedAtRestDoItVector)[fAtRestDoItProcTriggered] = 1;

   fStep->SetStepLength( 0. );  //the particle has stopped
   fTrack->SetStepLength( 0. );

// invoke selected process
   for(size_t np=0; np < MAXofAtRestLoops; np++){
   //
   // Note: DoItVector has inverse order against GetPhysIntVector
   //       and SelectedAtRestDoItVector.
   //
     if( (*fSelectedAtRestDoItVector)[MAXofAtRestLoops-np-1] != 0){


       fCurrentProcess = (*fAtRestDoItVector)[np];
       fParticleChange 
         = fCurrentProcess->AtRestDoIt( *fTrack, *fStep);
                               
       // Set the current process as a process which defined this Step length
       fStep->GetPostStepPoint()
              ->SetProcessDefinedStep(fCurrentProcess);

       // Update Step  
       fParticleChange->UpdateStepForAtRest(fStep);

       // Now Store the secondaries from ParticleChange to SecondaryList
       G4Track*  tempSecondaryTrack;
       G4bool tApplyCutFlag;
       G4double tProdThreshold;
       G4Material* tMaterial;
       G4double tBelowCutEnergyAndSafety;

       fN2ndariesAtRestDoIt = fParticleChange->GetNumberOfSecondaries();

       for(G4int DSecLoop=0 ; DSecLoop< fN2ndariesAtRestDoIt; DSecLoop++){
         tempSecondaryTrack = fParticleChange->GetSecondary(DSecLoop);
         tApplyCutFlag = tempSecondaryTrack->GetDefinition()
                                               ->GetApplyCutsFlag();

         // Check if the particle should be passed without coherent cut
         if(tApplyCutFlag){
            tBelowCutEnergyAndSafety = false;            
            tMaterial = fPostStepPoint->GetMaterial();
            tProdThreshold = tempSecondaryTrack->GetDefinition()
                             -> GetEnergyThreshold(tMaterial);

            if( tempSecondaryTrack->GetKineticEnergy()<tProdThreshold ){
              G4double currentRange
                    = G4EnergyLossTables::GetRange(
                                tempSecondaryTrack->GetDefinition(),
				tempSecondaryTrack->GetKineticEnergy(),
				tMaterial
			);	
              if (currentRange < CalculateSafety() ){
                 tBelowCutEnergyAndSafety = true;
              }
            }

            if( tBelowCutEnergyAndSafety ){
                if( !(tempSecondaryTrack->IsGoodForTracking()) ){

////                   G4cout << "!! Warning - G4SteppingManager:" << G4endl; 
////                        << " This physics process generated a secondary"
////                        << " of which energy is below cut but" 
////                        << " GoodForTracking is off !!!!!" << G4endl;

                   // Add kinetic energy to the total energy deposit
                   fStep->AddTotalEnergyDeposit(
                          tempSecondaryTrack->GetKineticEnergy() );
                   tempSecondaryTrack->SetKineticEnergy(0.0);
                } 
            }
         }

         // If this 2ndry particle has 'zero' kinetic energy, make sure
         // it invokes a rest process at the beginning of the tracking
         if(tempSecondaryTrack->GetKineticEnergy() <= 0.){
	    tempSecondaryTrack->SetTrackStatus( fStopButAlive );
         }

         // Set parentID 
         tempSecondaryTrack->SetParentID( fTrack->GetTrackID() );

	 // Set the process pointer which created this track 
	 tempSecondaryTrack->SetCreatorProcess( fCurrentProcess );

	 fSecondary->push_back ( tempSecondaryTrack );
       }

       // clear ParticleChange
       fParticleChange->Clear();

     } //if(fSelectedAtRestDoItVector[np] != 0){
   } //for(size_t np=0; np < MAXofAtRestLoops; np++){

   fStep->UpdateTrack();

   fTrack->SetTrackStatus( fStopAndKill );
}


/////////////////////////////////////////////////////////
void G4SteppingManager::InvokeAlongStepDoItProcs()
/////////////////////////////////////////////////////////
{

// If the current Step is defined by a 'ExclusivelyForced' 
// PostStepDoIt, then don't invoke any AlongStepDoIt
   if(fStepStatus == fExclusivelyForcedProc){
     return;               // Take note 'return' at here !!!
   }

// Invoke the all active continuous processes
   fN2ndariesAlongStepDoIt = 0;

   for( size_t ci=0 ; ci<MAXofAlongStepLoops ; ci++ ){
     fCurrentProcess = (*fAlongStepDoItVector)[ci];
     if (fCurrentProcess== NULL) continue;
         // NULL means the process is inactivated by a user on fly.

     fParticleChange 
       = fCurrentProcess->AlongStepDoIt( *fTrack, *fStep );

     // Update the PostStepPoint of Step according to ParticleChange
     fParticleChange->UpdateStepForAlongStep(fStep);
#ifdef G4VERBOSE
                         // !!!!! Verbose
               if(verboseLevel>0) fVerbose->AlongStepDoItOneByOne();
#endif

     // Now Store the secondaries from ParticleChange to SecondaryList
     G4Track* tempSecondaryTrack;
     G4bool tApplyCutFlag;
     G4double tProdThreshold;
     G4Material* tMaterial;
     G4double tBelowCutEnergyAndSafety;

     fN2ndariesAlongStepDoIt = fParticleChange->GetNumberOfSecondaries();

     for(G4int DSecLoop=0 ; DSecLoop< fN2ndariesAlongStepDoIt; DSecLoop++){
         tempSecondaryTrack = fParticleChange->GetSecondary(DSecLoop);
         tApplyCutFlag = tempSecondaryTrack->GetDefinition()
                                               ->GetApplyCutsFlag();

         // Check if the particle should be passed without coherent cut
         if(tApplyCutFlag){
            tBelowCutEnergyAndSafety = false;            
            tMaterial = fPostStepPoint->GetMaterial();
            tProdThreshold = tempSecondaryTrack->GetDefinition()
                             -> GetEnergyThreshold(tMaterial);

            if( tempSecondaryTrack->GetKineticEnergy()<tProdThreshold ){
              G4double currentRange
                    = G4EnergyLossTables::GetRange(
                                tempSecondaryTrack->GetDefinition(),
				tempSecondaryTrack->GetKineticEnergy(),
				tMaterial
			);	
              if (currentRange < CalculateSafety() ){
                   tBelowCutEnergyAndSafety = true;
              }
            }

            if( tBelowCutEnergyAndSafety ){
                if( !(tempSecondaryTrack->IsGoodForTracking()) ){

////                   G4cout << "!! Warning - G4SteppingManager:" << G4endl; 
////                        << " This physics process generated a secondary"
////                        << " of which energy is below cut but" 
////                        << " GoodForTracking is off !!!!!" << G4endl;

                   // Add kinetic energy to the total energy deposit
                   fStep->AddTotalEnergyDeposit(
                          tempSecondaryTrack->GetKineticEnergy() );
                   tempSecondaryTrack->SetKineticEnergy(0.0);
                } 
            }
         }

         // If this 2ndry particle has 'zero' kinetic energy, make sure
         // it invokes a rest process at the beginning of the tracking
            if(tempSecondaryTrack->GetKineticEnergy() <= 0.){
              tempSecondaryTrack->SetTrackStatus( fStopButAlive );
            }

         // Set parentID
         tempSecondaryTrack->SetParentID( fTrack->GetTrackID() );

	 // Set the process pointer which created this track 
	 tempSecondaryTrack->SetCreatorProcess( fCurrentProcess );

	 fSecondary->push_back(  tempSecondaryTrack );
     }

     // Set the track status according to what the process defined
     fTrack->SetTrackStatus( fParticleChange->GetStatusChange() );

     // clear ParticleChange
     fParticleChange->Clear();
   }
}

////////////////////////////////////////////////////////
void G4SteppingManager::InvokePostStepDoItProcs()
////////////////////////////////////////////////////////
{

// Invoke the specified discrete processes
   fN2ndariesPostStepDoIt = 0;

   for(size_t np=0; np < MAXofPostStepLoops; np++){
   //
   // Note: DoItVector has inverse order against GetPhysIntVector
   //       and SelectedPostStepDoItVector.
   //
     size_t npGPIL = MAXofPostStepLoops-np-1;
     
     G4int tmp= (*fSelectedPostStepDoItVector)[npGPIL];
     if(tmp != 0){

       if ( ((tmp == 1) && (fStepStatus == fPostStepDoItProc)) ||
            ((tmp == 2) && (fStepStatus != fExclusivelyForcedProc)) ||
            ((tmp == 3) && (fStepStatus == fAlongStepDoItProc)) ||
	    ((tmp == 4) && (fStepStatus == fExclusivelyForcedProc)) ) {

         fCurrentProcess = (*fPostStepDoItVector)[np];
         fParticleChange 
            = fCurrentProcess->PostStepDoIt( *fTrack, *fStep);

         // Update PostStepPoint of Step according to ParticleChange
	 fParticleChange->UpdateStepForPostStep(fStep);
#ifdef G4VERBOSE
                 // !!!!! Verbose
           if(verboseLevel>0) fVerbose->PostStepDoItOneByOne();
#endif
         // Update G4Track according to ParticleChange after each PostStepDoIt
         fStep->UpdateTrack();

         // Update safety after each invocation of PostStepDoIts
         fStep->GetPostStepPoint()->SetSafety( CalculateSafety() );

         // Now Store the secondaries from ParticleChange to SecondaryList
         G4Track* tempSecondaryTrack;
         G4bool tApplyCutFlag;
         G4double tProdThreshold;
         G4Material* tMaterial;
         G4double tBelowCutEnergyAndSafety;

         fN2ndariesPostStepDoIt = fParticleChange->GetNumberOfSecondaries();

         for(G4int DSecLoop=0 ; DSecLoop< fN2ndariesPostStepDoIt; DSecLoop++){
            tempSecondaryTrack = fParticleChange->GetSecondary(DSecLoop);
            tApplyCutFlag = tempSecondaryTrack->GetDefinition()
                                               ->GetApplyCutsFlag();

            // Check if the particle should be passed without coherent cut
            if(tApplyCutFlag){
               tBelowCutEnergyAndSafety = false;            
               tMaterial = fPostStepPoint->GetMaterial();
               tProdThreshold = tempSecondaryTrack->GetDefinition()
                                -> GetEnergyThreshold(tMaterial);

               if( tempSecondaryTrack->GetKineticEnergy()<tProdThreshold ){
                 G4double currentRange
                    = G4EnergyLossTables::GetRange(
                                tempSecondaryTrack->GetDefinition(),
				tempSecondaryTrack->GetKineticEnergy(),
				tMaterial
			);	
                 if (currentRange < CalculateSafety() ){
                   tBelowCutEnergyAndSafety = true;
                 }
               }

               if( tBelowCutEnergyAndSafety ){
                   if( !(tempSecondaryTrack->IsGoodForTracking()) ){

////                      G4cout << "!! Warning - G4SteppingManager:" << G4endl; 
////                           << " This physics process generated a secondary"
////                           << " of which energy is below cut but" 
////                           << " GoodForTracking is off !!!!!" << G4endl;

                      // Add kinetic energy to the total energy deposit
                      fStep->AddTotalEnergyDeposit(
                             tempSecondaryTrack->GetKineticEnergy() );
                      tempSecondaryTrack->SetKineticEnergy(0.0);
                   } 
               }
            }

            // If this 2ndry particle has 'zero' kinetic energy, make sure
            // it invokes a rest process at the beginning of the tracking
            if(tempSecondaryTrack->GetKineticEnergy() <= 0.){
   	      tempSecondaryTrack->SetTrackStatus( fStopButAlive );
            }

            // Set parentID 
            tempSecondaryTrack->SetParentID( fTrack->GetTrackID() );
	    
	    // Set the process pointer which created this track 
	    tempSecondaryTrack->SetCreatorProcess( fCurrentProcess );

	    fSecondary->push_back( tempSecondaryTrack );
         }

         // Set the track status according to what the process defined
         fTrack->SetTrackStatus( fParticleChange->GetStatusChange() );

         // clear ParticleChange
         fParticleChange->Clear();
       }
     } //if(*fSelectedPostStepDoItVector(np) != 0){

   // Exit from PostStepLoop if the track has been killed
   if(fTrack->GetTrackStatus() == fStopAndKill) break;

   } //for(size_t np=0; np < MAXofPostStepLoops; np++){
}
