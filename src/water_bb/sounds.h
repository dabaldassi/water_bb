#ifndef SOUNDS_H
#define SOUNDS_H

/*** MUSIC ***/

#define STR(D,M) #D#M
#define BUILD_MUSIC(D,M) STR(D,M)
#define MK_MUSIC(M) BUILD_MUSIC(MUSIC_DIR,M)
#define MUSIC_DIR ../assets/musics/


#define MUSIC_MAIN_PHASE MK_MUSIC(music_main_phase.wav)
#define STORY MK_MUSIC(story2.wav)


/*** SOUND ***/

#define BUILD_SOUND(D,S) STR(D,S)
#define SOUND_DIR ../assets/sounds/
#define MK_SOUND(S) BUILD_SOUND(SOUND_DIR,S)

#define DEPART1 MK_SOUND(départ1.wav)
#define DEPART2 MK_SOUND(départ2.wav)
#define DEPART3 MK_SOUND(départ3.wav)
#define DEPART4 MK_SOUND(départ4.wav)
#define DEPART5 MK_SOUND(départ5.wav)
#define DEPART6 MK_SOUND(départ6.wav)

#define RAVITAILLEMENT1 MK_SOUND(ravitaillement1.wav)
#define RAVITAILLEMENT2 MK_SOUND(ravitaillement2.wav)
#define RAVITAILLEMENT3 MK_SOUND(ravitaillement3.wav)
#define RAVITAILLEMENT4 MK_SOUND(ravitaillement4.wav)

#define SELECTION1 MK_SOUND(sélection1.wav)
#define SELECTION2 MK_SOUND(sélection2.wav)
#define SELECTION3 MK_SOUND(sélection3.wav)
#define SELECTION4 MK_SOUND(sélection4.wav)
#define SELECTION5 MK_SOUND(sélection5.wav)

#define MOVE_ERR MK_SOUND(moveerr.wav)
#define RABBIT_ALERT MK_SOUND(recuplapin.wav)
#define COLLISION_BOAT MK_SOUND(collision.wav)
#define DEADBOAT MK_SOUND(deadboat.wav)
#define FIRE MK_SOUND(fire.wav)
#define SHUFFLE MK_SOUND(shuffle.wav)
#define THUNDER MK_SOUND(thunder.wav)
#define CHANGE_TURN MK_SOUND(tour.wav)
#define WIND MK_SOUND(vent.wav)
#define CURSOR MK_SOUND(curseur.wav)

#endif /* SOUNDS_H */
