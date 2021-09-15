#include "OEMSound.h"
#include "AEE_OEMSound.h"

void OEMSound_Init(void) {

}

int  OEMSound_NewInstance(AEESoundInfo * psi) {
    return SUCCESS;
}

void OEMSound_DeleteInstance(AEESoundInfo * psi) {
    // return SUCCESS;
}

void OEMSound_SetDevice(AEESoundInfo * psi, void * pUser) {
    AEESound_StatusCB(pUser, AEE_SOUND_FAILURE);
}

void OEMSound_Get(AEESoundInfo * psi) {

}

void OEMSound_PlayTone(AEESoundInfo * psi, AEESoundToneData * toneData, void * pUser) {
    AEESound_StatusCB(pUser, AEE_SOUND_FAILURE);
}

void OEMSound_PlayFreqTone(AEESoundInfo * psi, uint16 wHiFreq, uint16 wLoFreq, uint16 wDuration, void * pUser) {
    AEESound_StatusCB(pUser, AEE_SOUND_FAILURE);
}

void OEMSound_PlayToneList(AEESoundInfo * psi, AEESoundToneData * pToneData, uint16 wDataLen, void * pUser) {
    AEESound_StatusCB(pUser, AEE_SOUND_FAILURE);
}

void OEMSound_StopTone(boolean bPlayList, void * pUser) {
    AEESound_StatusCB(pUser, AEE_SOUND_FAILURE);
}

void OEMSound_GetLevels(AEESoundInfo * psi, void * pUser) {
    AEESound_LevelCB(pUser, AEE_SOUND_FAILURE, 100);
}

void OEMSound_SetVolume(AEESoundInfo * psi, uint16 wLevel, void * pUser) {
    AEESound_VolumeCB(pUser, AEE_SOUND_FAILURE, 100);
}

void OEMSound_GetVolume(AEESoundInfo * psi, void * pUser) {
    AEESound_VolumeCB(pUser, AEE_SOUND_FAILURE, 100);
}

void OEMSound_Vibrate(uint16 wDuration, void * pUser) {
    AEESound_StatusCB(pUser, AEE_SOUND_FAILURE);
}

void OEMSound_StopVibrate(void * pUser) {
    AEESound_StatusCB(pUser, AEE_SOUND_FAILURE);
}
