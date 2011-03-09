#ifndef __SETUP_
#define __SETUP_

#define POCETLISTU 8
#define POCETKLAVES 128
#define POCETAKTICNICHKLAVES 40

typedef struct
{
	char	no_sound;
	char	no_menu;
	char	camera_type;
	char	log_file[256];
	int		xres;
	int		yres;
	char	bpp;
	char	fullscreen;
	char	ditering;
	char	barva_pozadi;		//TF
	char	strip;				//TF
	char	typ_mlhy;			//0 = vertex, 1 - pixel
	char	text_detail;        //Texture-level - Uroven detailu textur (0 = max)
	char	text_detail_light;  //Uroven detailu textur lightmap (0 = max)
	char	text_kvalita;		//OpenGL - Nice = 1/Fast = 0 TF
	char	text_komprese;		//Komprese do DXT formatu, pokud to karta povoluje TF
	char	text_mip_mapping;	//Generovat pro startu mip-mapy + jejich povoleni  TF
	char	text_mip_filtr;		//Filtr mip-map 0 - linearni/1 - trilinearni TF
	char	text_bpp;			//Hloubka textur 16/32 bitu
	char	channels;
	long	pre_load;
	char	soundvolume;
	char	musicvolume;
	char	ambientvolume;
	int		key[POCETKLAVES];
	char	bitmap_dir[256];
	char	data_dir[256];
	char	prvky_dir[256];
	char	level_dir[256];
	char	out_dir[256];
	char	texture_dir[256];
	char	material_dir[256];
	char	game_level_dir[256];
	char	scene_dir[256];
	char	game_data_dir[256];
	char	music_dir[256];
	char	sound_dir[256];
	char	save_dir[256];
	char	e_ditering;
	char	mazat_dummy;
	char	kolize_materialu;
	char	load_prvky;
	char	load_textury;
	char	load_default_materialy;
	char	default_materialy[256];
	char	obalky_kontejner;
	char	obalky_objekt;
	char	berusky_hra[256];
	char	e_log_file[256];
	char	debug_vypisy;
	char	debug_light;
	char	pohled_berusky;
	char	kvalita_castic;
	char	kvalita_casticv;
	char	kvalita_casticp;
	char	ovladani;
	char	debug_fps;
	char	lista_prvku;
	char	berusky_hra_dir[256];
	char	debug_draty;
	char	sada[20][256];
	char	posouvat_kameru;
	char	extra_light_vertex;
	float	text_ostrost;
	float	p_kamera_alfa;
	float	p_kamera_radius;
	char	init_visibility;
	char	cursor[32];
	char	text_bpp_plocha;
	char	hires_textury;
	char	menu_vram_load;
	char	zrcado_aktivni;
	char	debug_kamery;
	char	text_ans;
	float	text_ans_level;
	char	bump_mapping;
	int		bump_mapping_typ;
	int		text_detail2;
	int		text_detail3;
	int		light_dyn;
	int		light_rychlost;
	int		text_bump2;
	int		text_bump3;
	int		scene_materialy;
	int		text_ans_stupen;
	int		vertex_arrays;
	int		start_zpruhlednovani;
	int		start_zvyraznovani;
	int		ovladani_rohy;
	int		ovladani_rohy_default;
	int		ovladani_rohy_smer;
	float	ovladani_rohy_rychlost;
	int		ovladani_pr_posun;
	int		animace_okoli;
	int		general_3d;
	char	camera_intro;
} SETUP;

typedef struct
{
	DWORD x;
	DWORD y;
	DWORD bpp;
	DWORD freq;
} ROZLISENI;

void Load_ini(void);
void Save_ini(void);

int GetResCombobox(ROZLISENI *p_roz, int roz_size);

#endif
