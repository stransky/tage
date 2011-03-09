/*
  Oktanovy stromy

  Scena + okoli -> KD strom
  Prvky -> obalky

*/
#ifndef __KDTREE_H__
#define __KDTREE_H__

#define KD_FULL_VISIBILITY 8

/*
  Bunka -> min/max
  - zpusob rozdeleni (x/y/z)
  - vzdalenost delici roviny

  - Prusecik focus s tim ctvercem (ray-tracing)
  - prusecik s kamerou

*/

#define  KD_POLY      1
#define  KD_MESH      2

typedef struct _KD_BUNKA {

  int                mail;
  int                polozek;
  void             **p_polozka; //pole polozek (poly/kont)
  int               *p_polozka_typ; // typy polozek (kontejner/poly)
  BOD                min,max;
  int                rovina;    // delici rovina podstromu
  float              vzdal;     // vzdalenost od min
  int                cislo;
  struct _KD_BUNKA  *p_up;      // otec stromu
  struct _KD_BUNKA  *p_next;    // dalsich 2 podstromu nebo nic

} KD_BUNKA;


void  kd_strom_vyrob(EDIT_MESH_POLY *p_poly, int polynum, EDIT_KONTEJNER **p_kont, int kontnum, KD_BUNKA *p_prvni);
void  kd_strom_tiskni(KD_BUNKA *p_prvni, int hloubka);
int   kd_visibility(BOD *p_min, BOD *p_max, GLMATRIX *p_m);
int   kd_visibility_full(BOD *p_min, BOD *p_max, GLMATRIX *p_m);
int   kd_intersect_kostku(BOD *p_a, BOD *p_b, BOD *p_min, BOD *p_max);
int   kd_intersect_kostku_dir(BOD *p_orig, BOD *p_dir, BOD *p_min, BOD *p_max);
int   kd_intersect_kostku_bod(BOD *p_a, BOD *p_b, BOD *p_min, BOD *p_max, BOD *p_p, float *p_t);
int   kd_intersect_kostku_bod_inter(BOD *p_a, BOD *p_b, BOD *p_min, BOD *p_max, BOD *p_p, float *p_t);
void  kresli_obalku(BOD min, BOD max, dword barva, GLMATRIX *p_tr);

#endif
