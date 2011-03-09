/*
  Header k enginu
*/
#ifndef __MAT_3D_H__
#define __MAT_3D_H__


#define  T_WRAP          0x1
#define  T_MIRROR        0x2

#define  K_CHYBA         (-1)
#define  NIC             K_CHYBA

#ifndef GK_LOOP
#define GK_LOOP          0x100   // Run-animace se prehrava furt dokola
#endif

#ifndef GK_REMOVE
#define GK_REMOVE        0x200   // Animace se po dobehnuti automaticky smaze
#endif


#define STATE_ADD        2
#define STATE_PRUHLEDNY  3


/* Definice typu
*/
typedef struct _MUJ_BOD {
 
  float x, y, z;
  float px,py,pz;
  float nx,ny,nz;
  
  float mdr,mdg,mdb,mda;
  float dr ,dg ,db ,da;
  float msr,msg,msb;
  float sr ,sg ,sb;
  
  float tu1,tv1; // 4 textury
  float tu2,tv2;
  float tu3,tv3;
  float tu4,tv4; // lightmapa
  float tul,tvl;

} MUJ_BOD,OBJ_VERTEX,OBJEKT_BOD;

/* Definice vertexovych poli
*/
typedef struct _VERTEX_ARRAYS {

  dword ati_handle;      // handle na ati vertex-pole    
  dword ati_velikost;    // velikost pole - pro cely objekt
  dword ati_start_pos;
  dword ati_start_norm;
  dword ati_start_diff;
  dword ati_start_spec;
  dword ati_start_text[MAT_TEXTUR];

  dword arb_handle_indicie;

} VERTEX_ARRAYS;

typedef struct _NORMALBOD {
 
  float x,y,z;
  dword diff;

} NORMALBOD;

typedef struct _MUJ_BOD_FLARE {
 
  float x,y,z,rhw;
  dword diff;
  float tu1,tv1;

} MUJ_BOD_FLARE;

typedef struct _DXBOD {
  
  float x,y,z;
  float r,g,b;
  
} DXBOD;




typedef struct _ZDRCADLO_DESC_POLY {

  int        zrcadlo_k; // kontejner zrcadla
  int        zrcadlo_o; // objekt zrcadla
  int        id_kont;   // id kontejneru
  int        poly;      // poly ktereho se to tyka
  
  struct    _ZDRCADLO_DESC_POLY *p_next;

} ZDRCADLO_DESC_POLY;


typedef struct _ZDRCADLO_DESC {

  // Zustava - popisuje rovine zrcadlo
  GLMATRIX   ref;       // reflexni matice
  BOD        p[4];      // 4 body roviny zrcadla
  ROVINAD    r[5];      // 4 plochy klipovaci pyramidy

  ZDRCADLO_DESC_POLY *p_poly;

} ZDRCADLO_DESC;


// Multitextuing pro 1 texturu
typedef struct _MULTITEXT_FLAG {

  GLenum cil;
  GLenum mod;
  GLint  param;
    
} MULTITEXT_FLAG;

typedef struct _OLD_MULTITEXT_FLAG {

  dword a_arg1,a_op,a_arg2;
  dword c_arg1,c_op,c_arg2;
  dword src,dest;

} OLD_MULTITEXT_FLAG;


/*
#define  MLHA_AKTIVNI    0x1

typedef struct _MLZNA_KOSTKA {

  char  jmeno[MAX_JMENO];// jmeno mlzne kostky
  BOD   min,max;         // rozmery kostky
  int   flag;            // flagy mlhy
  int   mod;             // mod mlhy (linear/exp/exp2)
  int   priorita;        // priorita mlzne kostky
  float start;           // zacatek
  float stop;            // konec
  float intenzita;       // intenzita
  float r,g,b,a;         // barva
  void *p_kont;          // pointer na kontejner ke kteremu je privazana
  int   poly;            // zazba na poly

  struct _MLZNA_KOSTKA *p_next;

} MLZNA_KOSTKA;
*/

#define  SL2_DYNAMIC    0x1        // svetlo se aplikuje i na dynamicke veci
#define  SL2_MESH       0x2        // svetlo pouziva mesh

typedef struct _STATIC_LIGHT {

  int       akt;         // aktivni svetlo
  char      jmeno[MAX_JMENO]; // jmeno svetla
  int       index;       // index svetla (pozice + ID)  
  BOD       p;
  float     r,g,b;
  float     k1,k2,k3;    // tlumici koeficienty
  float     dosah;       // max. dosah svetla
  byte      uhel;        // tlumit podle uhlu svetla
  byte      utlum;       // tlumit podle vzdalenosti
  void     *p_flare;     // flare svetla - pointer na nej
  int       last_poly;   // posledni hitovany poly
  int       last_kont;   // posledni hitovany kontejner
  int       flag;        // flag vlasnosti svetla -> update a podobne
  int       flag2;       // druhy flag -> vlastnosti
  void     *p_mesh_data; // privazany mesh, ke kterymu to patri
  int       bez_prev;    // vyjmout z dyn osvetlovani

  struct _STATIC_LIGHT *p_next; // nasledujici svetlo (v seznamu u kontejneru)
  struct _STATIC_LIGHT *p_prev; // predchozi svetlo

} STATIC_LIGHT;

typedef struct _DYN_LIGHT_ANIM {

  dword          endtime;      // posledni cas

  int            pos_keys;     // klice posunu
  KEY_POINT_BRS *p_pkeys;
  BOD           *p_pos;
  
  int            trg_keys;     // klice cile
  KEY_POINT_BRS *p_tkeys;
  BOD           *p_trg;
  
  int            dos_keys;     // klice dosah/min/max
  KEY_POINT_BRS *p_dskeys;
  BOD           *p_dosah;

  int            diff_keys;    // diffusni klice
  KEY_POINT_BRS *p_dfkeys;
  RGBA    *p_diff;  

  int            alfa_keys;    // alfa klice
  KEY_POINT_BRS *p_akeys;
  float         *p_alfa;

  int            spec_keys;    // spekularni klice
  KEY_POINT_BRS *p_spkeys;
  RGBA          *p_spec;

} DYN_LIGHT_ANIM;

#define SDL_MESH            0x00000100 // svetlo patri k urcitemu meshi
#define SDL_ADD_SUB         0x00000080 // svetlo pracuje v komplementarnim add/sub modu
#define SDL_SUB             0x00000200 // svetla se nepricitaji ale moduluji
#define SDL_UTLUM_LINEAR    0x00000400 // linearni utlum - defaultni
#define SDL_UTLUM_KVADRATIC 0x00000800 // kvadraticky utlum

typedef struct _DYN_LIGHT {

  char   jmeno[MAX_JMENO];

  int    aup;         // aktualni upload
  float  avzdal;      // aktualni vzdalenost
  int    akt;         // svetlo je aktivni/neaktivni
  
  dword  flag;        // flagy svetla
  dword  ktrida;      // kofolova trida pouziti
  dword  pflag;       // editacni flagy svetla

  int    index;

  BOD    np;          // netransformovana pozice
  BOD    tp;          // transformovana pozice
  BOD    dir;         // umisteni a rozsahy svetla
  float  look_max;    // konec kuzele (default = 255 -> prepocitam podle vzdalenosi)
  float  dosah;       // konec svetla
  float  min;         // zacatek tlumeni kuzele
  float  max;         // konec tlumeni kuzele
  float  theta;       // sirka kuzele svetla
  
  float  dr,dg,db;    // diffusni barva svetla ve floatu    
  float  sr,sg,sb;    // diffusni barva svetla ve floatu - odecitaci
  void  *p_mesh_data; // pointer meshe, ke kteremu je svetlo pripojeny

  dword  time;
  dword  time_stop;
  dword  time_start;
  dword  time_delka;
  dword  start;
  dword  stop;
  int   *p_flag;       // pointer kam hazu prubeh animace
  dword  an_flag;      // animacni flagy (loop/auto remove)
  DYN_LIGHT_ANIM an;

  struct _DYN_LIGHT *p_next;
  struct _DYN_LIGHT *p_prev;

} DYN_LIGHT;

#define EDL_BODOVE       0x00000001 // Bodove svetlo
//#define EDL_REFLEKTOR    0x00000002 // Reflektor
#define EDL_PLOSNE_X     0x00000004 // Sviti pouze plosne
#define EDL_PLOSNE_Y     0x00000008

#define EDL_PLOSNE_Z     0x00000010
#define EDL_MESH         0x00000020 // Svetlo je privazane na meshe
#define EDL_SCENA        0x00000040 // Svetlo je privazane na scenu
#define EDL_ZEMNI_MLHA   0x00000080 // je to zemni mlha

#define EDL_GOURAND      0x00000100 // pocitat uhly
#define EDL_UTLUM_LIN    0x00000200 // pocita se utlumove faktory
#define EDL_UTLUM_KVAD   0x00000400 // pocita se utlumove faktory
#define EDL_DOSAH        0x00000800 // svetlo ma omezeny dosah

#define EDL_MAPPING      0x00001000 // Pouzije se na texturove koordinaty
#define EDL_MESH_LIGHT   0x00002000 // Svetlo sviti pouze na vybrane meshe (seznam meshu)
#define EDL_PRUHL_LIGHT  0x00004000 // "Zpruhlednovaci" svetlo
#define EDL_CAMERA_POINT 0x00008000 // svetlo je umistene ke kamere

#define EDL_ALFA_MOD     0x00100000 // Alfa kanal se bude modulovat
#define EDL_ALFA_SET     0x00200000 // Alfa kanal se nastavi (prepise puvodni hodnoty)
#define EDL_ALFA_ADD     0x00400000 // Alfa kanal se pricte
#define EDL_ALFA_SUB     0x00800000 // Alfa kanal se odecita

#define EDL_SPEC_MOD     0x01000000 // Spekularni Sveto se bude modulovat
#define EDL_SPEC_SET     0x02000000 // Spekularni Svetlo se nastavi (prepise puvodni hodnoty)
#define EDL_SPEC_ADD     0x04000000 // Spekularni Svetlo se pricte
#define EDL_SPEC_SUB     0x08000000 // Spekularni Svetlo se odecita

#define EDL_ZMENA_CAMERA 0x10000000 // Pozemni mlha - pri zmene kamery
#define EDL_ANIMACE      0x20000000 // svetlo se prave animuje
#define EDL_ZMENA_LOKAL  0x40000000 // lokalni zmena svetla
#define EDL_ZMENA_GLOBAL 0x80000000 // globalni zmena svetla

typedef struct _EXTRA_DYN_LIGHT {
   
  char   jmeno[MAX_JMENO];

  int    akt;         // svetlo je aktivni/neaktivni
  dword  flag;        // flagy svetla
  dword  pflag;       // posunovaci flag
  dword  mflag;       // map-flagy svetla
  dword  ktrida;      // kofolova trida svetla

  int    index;

  BOD    tp;
  BOD    np;
  BOD    dir;
  float  dosah;       // dosah svetla
  float  theta;       // sirka kuzele svetla
  float  utlum;       // utlum svetla
  float  gourand;     // koeficient gourandu -> lesk
  
  //float  dr,dg,db,da; // diffusni barva svetla
  float  da;          // diffusni alfa svetla
  float  sr,sg,sb;    // diffusni barva svetla
  
  void  *p_mesh_data; // zavisly mesh

  int   *p_lmesh;     // seznam privazanych meshu
  int    lmeshnum;    // max pocet meshu
  int    lmeshakt;    // aktualni pocet meshu

  dword  time;
  dword  time_stop;
  dword  time_start;
  dword  time_delka;
  dword  start;
  dword  stop;
  int   *p_flag;       // pointer kam hazu prubeh animace
  dword  an_flag;      // animacni flagy (loop/auto remove)
  DYN_LIGHT_ANIM an;

  float  v_start;      // zemni mlha - parametry
  float  v_stop;
  float  h_start;
  float  h_stop;

  struct _EXTRA_DYN_LIGHT *p_next;
  struct _EXTRA_DYN_LIGHT *p_prev;

} EXTRA_DYN_LIGHT;



/* Run-time flagy
*/
#define KONT_POHYB              0x00000001  // Mesh se pohnul v poslednim tahu
#define KONT_VIDITELNY          0x00000002  // Mesh je viditelny
#define KONT_DRAW_SPEC          0x00000004  // Mesh/Poly pouziva na vykresleni docasne spekularni barvu
#define KONT_MPMATRIX           0x00000008  // kontejner pouziva MP matrix

#define KONT_DRAW_PRUHL         0x00000010  // Mesh/Poly se ma kreslit jako pruhledny material
#define KONT_DRAW_CAMERA        0x00000020  // Mesh/Poly je pred kamerou a ma se kreslit jako pruhledny
#define KONT_DRAW_LIGHT         0x00000040  // Mesh/Poly je privazan ke extra-svetlu
#define KONT_DRAW_NOBALKA       0x00000080  // Mesh/Poly nema obalku (nema se na ni testovat)

#define KONT_DRAW_VODA          0x00000100  // Mesh/Poly je voda (pouzivaji se 3.ti barevne slozky)
#define KONT_VIDITELNY_ZRC      0x00000200  // Mesh je viditelny v zrcadle
#define KONT_OBRUBA             0x00000400  // Mesh je obruba levelu
#define KONT_PODLOZKA           0x00000800  // Mesh je podlozka levelu

#define KONT_NO_SELF_SHADOW     0x00001000 // nevrha stiny na sebe
#define KONT_PLG_OFFSET         0x00002000 // Mesh pouziva polygon-offset
#define KONT_NORMAL_ANIM        0x00004000 // animace normalovych vektoru
#define KONT_VIDITELNY_PRUHL    0x00008000 // poly/mesh je viditelny a pruhledny

#define KONT_SVETLO             0x00010000 // kontejner ma pripojene svetlo
#define KONT_VODA               0x00020000 // kontejner ma pripojenou vodu/vlnovy generator
#define KONT_NORMAL_ANIM_POS    0x00040000 // animace normalovych vektoru
//#define KONT_VIDITELNY_ZRC_FULL 0x00040000 // Mesh je viditelny v zrcadle
#define KONT_UPLOAD             0x00080000 // Upload meshe do vertex-pole

#define KONT_NO_FOG             0x00100000 // - nekresli se mlha 
#define KONT_NO_CALC            0x00200000 // - je to needitovaelny objekt
#define KONT_POHYB_ZRUS         0x00400000 // zrusit pohybovej flag
#define KONT_STATIC             0x00800000 // je staticky

//#define KONT_LEVEL_CUBE         0x01000000 // je to kostka kolem levelu
#define KONT_BRUTALNORMAL       0x02000000 // uplne kulaty objekt
#define KONT_VIDITELNY_ZMENA    0x04000000 // zmenila se viditelnost meshe (z neviditelneho na viditelneho!)
#define KONT_PRVEK              0x08000000 // prvek levelu

#define KONT_VERTEX_LIGHT       0x10000000 // svetla ve vertexech
#define KONT_NO_DRAW_SHADOW     0x20000000 // nevrha stiny na dalsi objekty
#define KONT_KEYFRAME           0x40000000 // kontejner ma pripojene keyframy
#define KONT_ZRCADLO            0x80000000 // kontejner/poly je zrcadlo

#define KONT2_UPDATE_POS        0x00000001
#define KONT2_UPDATE_NORM       0x00000002
#define KONT2_UPDATE_DIFF       0x00000004
#define KONT2_UPDATE_SPEC       0x00000008
#define KONT2_UPDATE_TEXT1      0x00000010
#define KONT2_UPDATE_TEXT2      0x00000020
#define KONT2_UPDATE_TEXT3      0x00000040
#define KONT2_UPDATE_TEXT4      0x00000080
#define KONT2_UPDATE_NEW        0x00000100

#define KONT2_UPLOAD            (KONT2_UPDATE_POS|KONT2_UPDATE_NORM|\
                                 KONT2_UPDATE_DIFF|KONT2_UPDATE_SPEC|\
                                 KONT2_UPDATE_TEXT1|KONT2_UPDATE_TEXT2|\
                                 KONT2_UPDATE_TEXT3|KONT2_UPDATE_TEXT4|\
                                 KONT2_UPDATE_NEW)

#define KONT2_LOADED            0x00000100 // kontejner byl nahran z disku
#define KONT2_JOINT_ANIM        0x00000200 // kontejner ma joint animace
#define KONT2_NO_PHONG          0x00000400 // kontejner se renderuje bez phongu
#define KONT2_BERUSKA           0x00000800 // kontejner je beruska


#define KONT_STATICKY_PRVEK  (KONT_PRVEK|KONT_STATIC)
#define KONT_DYNAMICKY_PRVEK (KONT_PRVEK|KONT_VERTEX_LIGHT|KONT_NO_DRAW_SHADOW|KONT_NO_SELF_SHADOW)





#define PART_AKT   0x1   // castice je aktivni
#define PART_POHYB 0x2   // castice se pohybuje
#define PART_SCALE 0x4   // castice se roztahuje/smrstuje
#define PART_ANIM  0x8   // castice je animovana


/*
typedef struct _MESH_OBALKY {

 BOD lmin;      // lokalni obalky min (pouze vertexy)
 BOD lmax;      // lokalni obalky max 
 BOD gmin;      // globalni obalky min (transformovane pozicni matici meshe)
 BOD gmax;      // globalni obalky max

} MESH_OBALKY;
*/

// vyrobi list sousednosti plosek -> na filtraci
typedef struct _FACE_SOUS {

  word f[3]; // pokud face == K_CHYBA -> je to to samy

} FACE_SOUS;


#define O_INDEX_POCET   0
#define O_INDEX_TYP     1
#define O_INDEX_DATA    2

// Konstni animace objektu
typedef struct _EDIT_OBJEKT {
/*
  char          jmeno[MAX_JMENO];
  char          jmeno_parent[MAX_JMENO];
  int           Objekt_ID;        // ID objektu

  word          vertexnum; 
  OBJ_VERTEX   *p_vertex;
  BOD          *p_vertex_kosti; // kostni vertexy

  word          facenum;
  word         *p_face;
  dword        *p_fsous;
  GLenum        face_typ;    // typ facu

  int           optnum; // optimalizovane facy
  int          *p_opt;
  
  word          linenum;
  word         *p_line;
  GLenum        line_typ;    // typ car

  int           material;    // pouzity material (z tabulky materialu)
  int           material_sub;

  float         float_scale; // aktualni float-scale
  float         float_scale_last; // minuly float scale

  word          x_rect;     // x ctvercu objektu
  word          y_rect;     // y ctvercu objektu

  byte          kreslit;

  BOD           pivot;      // pivot-point objektu (relativni)

  byte          trans;      // transformovane svetlo (je preveden na billboard a spocitane okoli)
  dword         m1flag;     // flagy objektu
  dword         m2flag;     // flagy objektu
  dword         oflag;
  
//  TRACK_INFO   *p_track;    // track info k animaci  
  GLMATRIX      m;          // matrix k animacim
  
  OBB           obb;

  void                *p_tmp_top;  // pomocny pointer na kontejner
  struct _EDIT_OBJEKT *p_tmp_next; // pomocny pointer na dalsi objekt
  
  //JOINT_ANIMACE       *p_joit_animace; // pointer na joint-animace

  struct _EDIT_OBJEKT *p_lod;      // LOD objekty tohodlenctoho

//  SIMPLE_TRACK_INFO   *p_tmp_sim;
*/
} EDIT_OBJEKT;

typedef struct _BUNKA_ZEME {
  
  int  s;     // preskocit bodu
  int  dx,dy; // bodu
  word o;
    
} BUNKA_ZEME;


#define MAX_KONT_OBJEKTU   3000
#define MAX_KONT_TEXTUR    200
#define MAX_KONT_MATERIALU 200

typedef struct _EDIT_KONTEJNER {
/*
 char         jmeno[MAX_JMENO];
 int          kontejner_ID;  // identifikacni cislo kontejneru -> jedinecne pri ukldani sceny
 EDIT_OBJEKT *p_obj[MAX_KONT_OBJEKTU];
 
 GLMATRIX     world;     // hlavni modifikovaci matice (scale a pod -> nulova transfromace na stred)
 GLMATRIX     mpmatrix;  // pozicni matice - aktivni pouze pri ukladani/nahravani
 
 int          max_objektu;
 byte         kreslit;
 
 int          prvek;      // pokud prvek -> je to logicky prvek k beruskam == K_CHYBA -> neni to prvek

 dword        m1flag;     // slepenec vsech materialu
 dword        m2flag;     // slepenec vsech materialu
 dword        kflag;      // slepenec vsech objektu
 dword        k2flag;

 int          facu;
 int          bodu;
 int          objektu; 

 int          mailbox; 
 int          kont;
 int          low_id;     // identifikator low/top-kontejneru
 
 OBB           obb;

 MLZNA_KOSTKA *p_mlha;    // pointer na mlznou kostku kontejneru

 HIERARCHY_SIM   sim[KONT_MAX_ANIM];  // seznam animaci - sim-animace
 int             akt_animace;    // aktualne vybrana animace 

 struct _STATIC_LIGHT    *p_slight; // seznam statickych svetel
 struct _DYN_LIGHT       *p_dlight; // seznam dynamickych svetel
 struct _EXTRA_DYN_LIGHT *p_edlight; // seznam extra svetel

 struct _EDIT_KONTEJNER  *p_next;     // dalsi kontejner v multi-mesh kontejnerech
 struct _EDIT_KONTEJNER  *p_next_low; // dalsi kontejner v low hierarchii
 struct _EDIT_KONTEJNER  *p_next_top; // top-kontejner
*/
} EDIT_KONTEJNER;

// Seznam fleku v levelu
typedef struct _FLEK {

  BOD        p;            // pozice fleku
  BOD        nx,nz;        // normalovy vektor fleku
  float      u[4];         // mapovaci souradnice u
  float      v[4];         // mapovaci souradnice v
  int        material;     // material fleku  
  int        uroven;
  float      bias;
  int        mesh;
  int        flag;  

  struct _FLEK *p_next;
  struct _FLEK *p_prew;

} FLEK;


typedef struct _POZICE_BODU_2D {

  float x,y,z,w;  // transformovane souradnice
  
} POZICE_BODU_2D;

typedef struct _TEXT_KOORD {
 
  float     x,y,z;
  float     nx,ny,nz;

  float     tu1,tv1;  // souradnice prvni textury
  float     tu2,tv2;  // souradnice druhe textury
  float     tu3,tv3;  // souradnice druhe textury
  float     tu4,tv4;  // souradnice druhe textury
  float     tul,tvl;  // souradnice light-mapy
  
  float     dr, dg, db,  da;// difusni barva barva r g b a
  float     mdr,mdg,mdb,mda;// difusni barva barva r g b a
  float     sr, sg, sb;     // spekularni barva barva r g b
  float     msr,msg,msb;    // spekularni barva barva r g b  

} TEXT_KOORD;

typedef struct _LIGHTMAP_FACE {

  int      last_u;
  int      upload;      // face se ma uploadovat do videokarty
  BITMAPA *p_bmp;       // pointer na bitmapu textury
  BITMAPA *p_zal;       // pointer na zalozni bitmapu textury

  ROVINA   n,rv,ru;     // rovina plosky, rovina vektoru r, rovina vektoru u
  BOD      s0,s1,s2,s3; // body na koncich ligthmapy
  float    ddu,ddv;     // delka vektoru u,v
  
  int      u,v;         // start body lightmapy
  int      nu,nv;       // pocet kroku

} LIGHTMAP_FACE;

// pouze na staticke objekty
typedef struct _EDIT_MESH_POLY { //celej kontejner
  
 char         jmeno[MAX_JMENO]; // jmeno poly

 int          facenum;   // co face to objekt
 TEXT_KOORD  *p_koord;   // list texture koordinatu - plosky po 3

 LIGHTMAP_FACE *p_ind;   // lightmapovaci indicie, je jich facenum/3
 dword       *p_fsous;   // sousedni plosky
 dword        m1flag;    // flag materialu -> stejne jako u kontejneru
 dword        m2flag;    // flag mat 2 -> stejne jako u kontejneru
 dword        kflag;     // flag kontejneru
 dword        k2flag;     // flag kontejneru
 
 int          material;  // material poly
 int          kont;      // cislo kontejneru, ze ktereho bylo poly vyrobeno
 int          poly;      // poradove cislo poly v seznamu polyu

 MLZNA_KOSTKA *p_mlha;      // mlzna kostka polyho

 int          lightnum;    // pocet lightmap
 int         *p_lightnum;  // pocty light-map
 EDIT_TEXT  **p_light;     // pole  light-map
 
 byte         kreslit;
 int          mail;        // mailbox poly :)

 OBB          obb;

 // Svetla poly - dynamicke 
 void       **p_dlight;  // pole svetel meshu 
 int          lightmax;  // velikost pole na svetla
 int          lightakt;  // aktualni vekikost

 // Svetla poly - extra-dynamicke
 int          top_edlight;// top-svetlo
 void       **p_edlight;  // pole extra svetel meshu 
 int          edlightmax; // velikost pole na extra svetla
 int          edlightakt; // aktualni velikost

 VERTEX_ARRAYS varray;    // vertex-arrays

} EDIT_MESH_POLY;


typedef struct _PRVEK_DATABAZE {  // prvek, ktery je v aktivni databazi prvku
  
  int kont;
  int guid;
  int kof_index;
  EDIT_KONTEJNER *p_kont;
  
} PRVEK_DATABAZE;

typedef struct _PRVEK_LEVELU {  // prvek, ktery je umisteny v levelu

  int       lev_index;             // pozice prvku v levelu
  int       prv_index;             // index v seznamu prvku v levelu
  GLMATRIX  m;                     // matrix kontejneru pokud je dynamicky
  PRVEK_DATABAZE *p_prv_databaze;  // odkaz do databaze prvku
  int       mesh;                  // cislo kontejneru pro kresleni prvku
  int       Vazba1;                // jedninecne vlastnosti prvku
  int       Vazba2;
  int       Rotace;
  int       guid;                  // guid prvku

} PRVEK_LEVELU;
/*
typedef struct _PRVEK_LEVELU_GAME {  // prvek, ktery je umisteny v levelu

  int        staticky;           // je staticky ?
  float      x,y,z;              // fyzicky 3D stred objektu
  int        rotace;             // natoceni prvku

  int        guid;               // guid prvku
  
  GLMATRIX   mg;                 // animacni matice (1 pri stani)
  GLMATRIX  *p_mg_top;           // top-animacni matice v anim strome
  GLMATRIX   mp;                 // pozicni matice (umistuje prvek na pozici)
  BOD        pivot;              // pivot-point pro animace (kopiruje se z animaci)
    
  GAME_MESH_DATA *p_mesh_data;     // mesh-data jsou stejne
  int        mesh;
  int        anim_mail;

  int        flag;               // flag prvku (low/top) prvek
  struct _PRVEK_LEVELU_GAME *p_next_low; // pointer na dalsi low/top prvek v poradi
  struct _PRVEK_LEVELU_GAME *p_prev_low; // pointer na predchozi low/top prvek v poradi

} PRVEK_LEVELU_GAME;
*/


#define SCENE_GAME_CUBE     0x1 // scena obsahuje game cube

#define COLOR_SET           0
#define COLOR_ADD           1
#define COLOR_SUB           2

#define DEFAULT_ALFA_FAKTOR 0xffffffff

typedef struct _TRIANGLE {
  OBJ_VERTEX a,b,c;
} TRIANGLE;

/*
typedef struct _KAMERA {

  int   cislo; // "use" - flag nebo K_CHYBA
  char  jmeno[MAX_JMENO]; // jmeno kamery
  int   flag;  // 0 - nic, 1,2 -> kreslit osy
  
  // Pocatecni hodnoty kamery
  
  BOD   p,t;   // umisteni kamery
  float fov;   // field-of-view
  float roll;  // nejakej jinej ?
  
  // Objekty kamery
  
  EDIT_OBJEKT *p_objekt; // objekt kamery
  EDIT_OBJEKT *p_target; // cil kam kamera ukazuje
  EDIT_OBJEKT *p_pos;    // pozice kamery

  // Animace kamery
  
  KAMERA_TRACK_INFO tracky[KAM_MAX_ANIM]; //animace kamer
  int               akt_animace; // aktualne vybrana animace kamery

} KAMERA;
*/

#define SLOZ_FLARE  14

typedef struct _LENS_FLARE_SLOZ {
  
  float vzdal;     // vzdalenost flare (0 = neaktivni)
  float r,g,b,a;   // barva lensu
  float dx,dy;     // rozmery dx,dy
  int   material;
  float wx,wy,wz;
  int   vid;
  EDIT_MATERIAL  *p_mat;  // material flaru

} LENS_FLARE_SLOZ;

typedef struct _LENS_FLARE {

  byte  akt;
  BOD   p;         // kde je ve 3d  
  BOD  *p_bod;     // pointer na bod
  float r,g,b,a;   // barva lensu
  float dx,dy;     // rozmery dx,dy
  int   material;
  EDIT_MATERIAL  *p_mat;  // material flaru
  int   index;

  STATIC_LIGHT *p_svetlo; // staticke svetlo flaru

  float wx,wy,wz;  // 2D souradnice
  int   vid;
  int   alfa;      // alfa mod
  int   rflag;     // renderivaci flag -> jak kreslit osy
  int   zflag;     // z-test flag

  LENS_FLARE_SLOZ *p_sloz; // zbytek slozenych flaru

  struct _LENS_FLARE *p_next; // pointer na dalsi flare
  struct _LENS_FLARE *p_prev; // pointer na predchozi flare

} LENS_FLARE;

// flagy pro lens-flary
#define  FLR_NO_ZTEST   0x1

typedef struct _ZEMNI_MLHA {

  int   aktivni;

  float v_start;
  float v_stop;

  float h_start;
  float h_stop;

  dword barva;

  int   handle;

} ZEMNI_MLHA;

typedef struct _LEVEL_KONFIG {

  dword      barva_pozadi; // pouzit barvu pozadi
  int        barva_pozadi_aktivni; // aktivni
  ZEMNI_MLHA zm;           // nastaveni zemni mlhy

} LEVEL_KONFIG;




__inline void transformuj_objekt_matici(EDIT_OBJEKT *p_obj, GLMATRIX *p_mat)
{/*
  int v;
  for(v = 0; v < p_obj->vertexnum; v++)
    transformuj_bod_matici(&p_obj->p_vertex[v].x,
                           &p_obj->p_vertex[v].y,
                           &p_obj->p_vertex[v].z,
                           p_mat);
                           */
}

__inline void nastav_material_rgb(RGB_M *p_rgb, float r, float g, float b)
{
 p_rgb->r = r;
 p_rgb->g = g;
 p_rgb->b = b;
}

// nastavi materialovou i vyslednou barvu
__inline void nastav_barvu_bodu_obj(EDIT_OBJEKT *p_obj, float r, float g, float b, float a)
{/*
  int v;
  for(v = 0; v < p_obj->vertexnum; v++) {
    p_obj->p_vertex[v].mdr = p_obj->p_vertex[v].dr = r;
    p_obj->p_vertex[v].mdg = p_obj->p_vertex[v].dg = g;
    p_obj->p_vertex[v].mdb = p_obj->p_vertex[v].db = b;
    p_obj->p_vertex[v].mda = p_obj->p_vertex[v].da = a;
  }
  */
}

__inline void nastav_barvu_bodu_obj_dword(EDIT_OBJEKT *p_obj, dword barva)
{/*
  int v;
  float b[4];
  rgba_float(barva,b);
  for(v = 0; v < p_obj->vertexnum; v++) {
    p_obj->p_vertex[v].mdr = p_obj->p_vertex[v].dr = b[0];
    p_obj->p_vertex[v].mdg = p_obj->p_vertex[v].dg = b[1];
    p_obj->p_vertex[v].mdb = p_obj->p_vertex[v].db = b[2];
    p_obj->p_vertex[v].mda = p_obj->p_vertex[v].da = b[3];
  }
  */
}

__inline void add_barvu_bodu_obj(EDIT_OBJEKT *p_obj, float r, float g, float b, float a)
{
  /*
  int v;
  for(v = 0; v < p_obj->vertexnum; v++) {    
    add_color_fa(&p_obj->p_vertex[v].mdr, r, g, b, a);
    add_color_fa(&p_obj->p_vertex[v].dr, r, g, b, a);
  }
  */
}

__inline void sub_barvu_bodu_obj(EDIT_OBJEKT *p_obj, float r, float g, float b, float a)
{/*
  int v;
  for(v = 0; v < p_obj->vertexnum; v++) {
    sub_color_fa(&p_obj->p_vertex[v].dr,r,g,b,a);
    sub_color_fa(&p_obj->p_vertex[v].mdr,r,g,b,a);
  }
  */
}


__inline void nastav_barvu_bodu_kont(EDIT_KONTEJNER *p_kont, float r, float g, float b, float a, byte op)
{/*
  int o,mo;

  mo = p_kont->max_objektu;
  for(o = 0; o < mo; o++) {
    if(p_kont->p_obj[o]) {
      switch (op) {
      case 0: //rep
        nastav_barvu_bodu_obj(p_kont->p_obj[o],r,g,b,a);
        break;
      case 1: //add
        add_barvu_bodu_obj(p_kont->p_obj[o],r,g,b,a);
        break;
      case 2: //sub
        sub_barvu_bodu_obj(p_kont->p_obj[o],r,g,b,a);
        break;
      }
    }
  }
  */
}
/*
__inline void nastav_barvu_bodu_kont_alfa(EDIT_KONTEJNER *p_kont, float a)
{
  int o;

  for(o = 0; o < p_kont->max_objektu; o++) {    
    if(p_kont->p_obj[o]) {
      oe_set_barva_objekt_maska(p_kont->p_obj[o],1,1,1,a,0,COLOR_SET,0,0,0,1);
    }
  }
}
*/

__inline void nastav_barvu_bodu_kont_dword(EDIT_KONTEJNER *p_kont, dword barva, byte op)
{/*
  int o,mo;
  float v[4];
  rgba_float(barva,v);
  mo = p_kont->max_objektu;
  for(o = 0; o < mo; o++) {
    if(p_kont->p_obj[o]) {
      switch (op) {
      case 0: //rep
        nastav_barvu_bodu_obj(p_kont->p_obj[o],v[0],v[1],v[2],v[3]);
        break;
      case 1: //add
        add_barvu_bodu_obj(p_kont->p_obj[o],v[0],v[1],v[2],v[3]);
        break;
      case 2: //sub
        sub_barvu_bodu_obj(p_kont->p_obj[o],v[0],v[1],v[2],v[3]);
        break;
      }
    }
  }
  */
}

extern GLMATRIX __kont_world_matrix;

__inline GLMATRIX * kont_world_matrix(EDIT_KONTEJNER *p_kont)
{/*
  if(p_kont->kflag&KONT_MPMATRIX) {    
    return(glmatrix::mult(&p_kont->world,&p_kont->mpmatrix,&__kont_world_matrix));
  } else {
    return(&p_kont->world);
  }
  */
}

#define DDRGB(r, g, b)     (0xff000000 | (((long)((b) * 255)) << 16) |(((long)((g) * 255)) << 8) | (long)((r) * 255))
#define DDRGBA(r, g, b, a) ((((long)((a) * 255)) << 24) | (((long)((b) * 255)) << 16) | (((long)((g) * 255)) << 8) | (long)((r) * 255))


void kont_norm_vect(EDIT_KONTEJNER *p_kont_top);
void transformuj_kontejner_matici(EDIT_KONTEJNER *p_kont, GLMATRIX *p_m);
void transformuj_kontejner_keyframe(EDIT_KONTEJNER *p_kont);
void transformuj_kontejner(EDIT_KONTEJNER *p_kont);
void calc_norm_vec_inter_object(EDIT_OBJEKT *p_obj);
//void mesh_norm_vect(GAME_MESH *p_mesh);
void texturuj_zemi(EDIT_OBJEKT *p_obj, int rect_x, int rect_y, int c_text, int flag, int mat, float skok_u, float skok_v);
EDIT_OBJEKT * vyrob_zemi(int rect_x, int rect_y, float skok);
EDIT_OBJEKT * vyrob_zemi_start(int rect_x, int rect_y, float skok, float start_x, float start_y);
void rozloz_zemi(EDIT_OBJEKT **p_src, int ctvercu);
EDIT_OBJEKT * mirror_objekt(EDIT_OBJEKT *p_src, int st, int mx, int my, int mz);
void mirror_text(EDIT_OBJEKT *p_src, int t, int mu, int mv);
void rotuj_text(EDIT_OBJEKT *p_obj, int t, int uhel);
void dopln_cary(EDIT_OBJEKT *p_obj);
void kresli_ctverec_2d(int x1, int y1, int x2, int y2, float r, float g, float b);
void kresli_ctverec_2d_plnej(int x1, int y1, int x2, int y2, float r, float g, float b);
void kresli_kurzor_mysi_kriz(int xres, int yres);
EDIT_OBJEKT * vyrob_krychli(BOD *p_bod, float x, float y, float z, dword barva);
EDIT_OBJEKT * vyrob_krychli_dratovou(BOD *p_bod, float x, float y, float z, dword barva);
void krychli_dratovou_recalc(EDIT_OBJEKT *p_obj, BOD *p_bod, float x, float y, float z, dword barva);
EDIT_OBJEKT * kameru_dratovou_calc(EDIT_OBJEKT *p_obj, BOD *p_p, BOD *p_t, float fov, float roll, dword barva);
EDIT_OBJEKT * kameru_dratovou_vyrob(BOD *p_p, BOD *p_t, float fov, float roll);
void invertuj_face_objektu(EDIT_OBJEKT *p_obj);
void invertuj_face_kontejner(EDIT_KONTEJNER *p_kont);
EDIT_KONTEJNER * vyrob_krychli_rozlozenou(BOD *p_bod, float x, float y, float z);
void pridej_krychli(EDIT_OBJEKT **p_src, BOD *p_bod, float x, float y, float z);
EDIT_OBJEKT * vyrob_billboard(BOD *p_bod, float x, float y);
void objekt_obalka(EDIT_OBJEKT *p_obj, GLMATRIX *p_m, BOD *p_min, BOD *p_max);
void kontejner_obalka(EDIT_KONTEJNER *p_kont);
int  cti_velikost_objektu(EDIT_OBJEKT *p_obj);
void cti_velikost_kontejneru(EDIT_KONTEJNER *p_kont, int *p_size, int *p_objektu);
int  intersect_ray_koule(BOD *p_stred, float radius, BOD *p_orig, BOD *p_dir);
void obj_vyrob_list_sousednosti(EDIT_OBJEKT *p_obj);
void obj_vyrob_list_sousednosti_full(EDIT_OBJEKT *p_obj);
void calc_norm_vec(EDIT_OBJEKT *p_obj);
void kresli_oktanovou_bunku(BOD *p_str, BOD *p_vel);
void kresli_oktanovou_bunku_minmax(BOD *p_min, BOD *p_max, dword barva);
void kresli_objekt_dratove_jednoduse(EDIT_OBJEKT *p_obj);
void nastav_material_rgb_MAT(RGB_M *p_rgb);
void nastav_material(float r, float g, float b);
EDIT_OBJEKT * umisti_krychli_dratovou(EDIT_OBJEKT *p_obj, BOD *p_bod);
void kresli_caru(BOD *p_s, BOD *p_k, dword barva);
void kresli_kosoctverec(GLMATRIX *p_project, GLMATRIX *p_camera, GLMATRIX *p_world, BOD *p_p, float r, float dr, float dg, float db);
void transformuj_objekt_text_coord(EDIT_OBJEKT *p_obj, GLMATRIX *p_mat, int coord);
void transformuj_kontejner_text_coord(EDIT_KONTEJNER *p_kont, GLMATRIX *p_mat, int coord);
void kresli_ctverec_2d_diff(int x1, int y1, int dx, int dy, float r, float g, float b);
int  stejny_vertex_point(BOD *v1, BOD *v2);
int  stejny_vertex_point_delta(BOD *v1, BOD *v2, float delta);
void mesh_env_maping_spec(GAME_MESH *p_mesh, GLMATRIX *p_cam, EDIT_MATERIAL **p_mt);
void poly_env_maping_spec(EDIT_MESH_POLY *p_poly, EDIT_MATERIAL *p_mat, GLMATRIX *p_cam);
void kresli_kosoctverec_word(BOD *p_p, float r, dword barva);
void poly_obalka(EDIT_MESH_POLY *p_poly, GLMATRIX *p_mat, BOD *p_min, BOD *p_max);
int  intersect_poly(EDIT_MESH_POLY *p_poly, BOD *p_orig, BOD *p_dir);
void kontejner_obalka_aabb(EDIT_KONTEJNER *p_kont, BOD *p_min, BOD *p_max);
//int  intersect_mesh(GAME_MESH *p_mesh, BOD *p_orig, BOD *p_dir);
int  intersect_mesh_objekt(GAME_MESH *p_mesh, int o, BOD *p_orig, BOD *p_dir);

__inline void oe_olist_reset(int *o)
{
  *o = -1;
}

__inline EDIT_OBJEKT * oe_olist_next(EDIT_KONTEJNER *p_kont, int *o)
{   /*
  EDIT_OBJEKT *p_obj;
  if(p_kont) {
    while((*o)+1 < MAX_KONT_OBJEKTU) {
      p_obj = p_kont->p_obj[++(*o)];
      if(p_obj)
        return(p_obj);      
    }
  }
  return(NULL);
  */
}

__inline float randf(void)
{
  return((float)rand()/(float)(RAND_MAX));
}

/*
  Look-up tabulka
*/
#define MAX_LOOK_TABLE 256
void konfiguruj_look_up(int *p_looksqrt_linear, int *p_looksqrt_quadratic, float *p_looksqrt_linear_float, float *p_looksqrt_quadratic_float);

// Minimalni intenzita svetla
#define DELTA_MIN 0.00392156f

//#define mmalloc(velikost) memset(malloc(velikost),0,velikost)

__inline void * kopiruj_pole(void *p_src, int velikost)
{
  if(p_src) {
    void *p_mem = mmalloc(velikost);
    memcpy(p_mem,p_src,velikost);
    return(p_mem);
  } else {
    return(NULL);
  }
}

#endif

