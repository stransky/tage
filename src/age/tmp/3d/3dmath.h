/*
  3D matematicke funkce
*/

#ifndef   __3D_MATH_H_
#define   __3D_MATH_H_

int  calc_prusek(ROVINA *r, OBJ_VERTEX *a, OBJ_VERTEX *b, OBJ_VERTEX *p);
int  calc_prusek_bod(ROVINA *r, BOD *a, BOD *b, BOD *p);

/* Bodove funkce
*/
__inline BOD * calc_face_normal(BOD *a, BOD *b, BOD *c, BOD *n)
{
 n->x = (b->y - a->y)*(c->z - a->z)-(c->y - a->y)*(b->z - a->z);
 n->y = (b->z - a->z)*(c->x - a->x)-(c->z - a->z)*(b->x - a->x);
 n->z = (b->x - a->x)*(c->y - a->y)-(c->x - a->x)*(b->y - a->y); 
 return(n);
}

__inline BOD * calc_face_stred(BOD *a, BOD *b, BOD *c, BOD *s)
{
 s->x = (a->x + b->x + c->x)/3.0f;
 s->y = (a->y + b->y + c->y)/3.0f;
 s->z = (a->z + b->z + c->z)/3.0f;
 return(s);
}

__inline GLMATRIX * calc_3d_2d_matrix(GLMATRIX *p_word, GLMATRIX *p_cam, GLMATRIX *p_proj, GLMATRIX *p_mat)
{ 
  return(glmatrix::mult(glmatrix::mult(p_word,p_cam,p_mat),p_proj,p_mat));
}

__inline void matrix_to_scale(GLMATRIX *p_m, BOD *p_s)
{
  p_s->x = vektor_norm_vect(&p_m->_11,&p_m->_21,&p_m->_31);
  p_s->y = vektor_norm_vect(&p_m->_12,&p_m->_22,&p_m->_32);
  p_s->z = vektor_norm_vect(&p_m->_13,&p_m->_23,&p_m->_33);
}

__inline void matrix_to_pos(GLMATRIX *p_m, BOD *p_p)
{
  p_p->x = p_m->_41;
  p_p->y = p_m->_42;
  p_p->z = p_m->_43;
}

__inline float matrix_to_float(GLMATRIX *p_mat)
{
  if(fabs(p_mat->_11) > fabs(p_mat->_21))
    return((float)acos(p_mat->_11));
  else
    return((float)asin(p_mat->_21));
}

__inline void matrix_to_scale_2d(GLMATRIX *p_m, BOD *p_s)
{
  p_s->x = vektor_norm_vect(&p_m->_11,&p_m->_21,&p_m->_31);
  p_s->y = vektor_norm_vect(&p_m->_12,&p_m->_22,&p_m->_32);
}

__inline void matrix_to_pos_2d(GLMATRIX *p_m, BOD *p_p)
{
  p_p->x = p_m->_41;
  p_p->y = p_m->_42;
}

__inline GLMATRIX * calc_transf_3d_2d_matrix(GLMATRIX *p_w, GLMATRIX *p_c, GLMATRIX *p_p, GLMATRIX *p_v)
{
 GLMATRIX m;
 return((GLMATRIX *)glu_invert_matrix((float *)glmatrix::mult(glmatrix::mult(p_w,p_c,p_v),p_p,&m),(float *)p_v));
}

__inline GLMATRIX * calc_transf_3d_2d_matrix_smpl(GLMATRIX *p_c, GLMATRIX *p_p, GLMATRIX *p_v)
{
 GLMATRIX m;
 return((GLMATRIX *)glu_invert_matrix((float *)glmatrix::mult(p_c,p_p,&m),(float *)p_v));
}

__inline void stred_to_obalka(BOD *p_prv, BOD *p_min, BOD *p_max, float dx, float dy, float dz)
{
  p_min->x = p_prv->x-dx;
  p_min->y = p_prv->y-dy;
  p_min->z = p_prv->z-dz;
  
  p_max->x = p_prv->x+dx;
  p_max->y = p_prv->y+dy;
  p_max->z = p_prv->z+dz;
}

#define EPSILON 0.000001f

__inline void kar2polar(BOD *p_car, float *p_r, float *p_fi, float *p_vzdal)
{
 BOD z = *p_car;
 float p; 
 *p_vzdal = vektor_norm(&z);
 *p_fi = asinf((fabsf(z.y) > 1.0f) ? ftoif(z.y) : z.y); 
 *p_r = fcopysign(acosf((fabsf(p = fchgsign(z.z)/cosf(*p_fi)) > 1.0f) ? ftoif(p) : p),z.x);
}

// polarni souradnice -> kartezske souradnice
__inline void polar2kar(float r, float fi, float vzdal, BOD *p_car)
{   
  p_car->x = vzdal*sinf(r)*cosf(fi);
  p_car->z = fchgsign(vzdal*cosf(r)*cosf(fi));
  p_car->y = vzdal*sinf(fi);
}

// odriznuti periody 2PI + normalizace na +PI..-PI
__inline float normalizuj_uhel(float uhel)
{
  if(fabs(uhel) > 2*PI) {
    uhel = (float)fmod((float)uhel,(float)2*PI);    
  }

  if(fabs(uhel) > PI) {
    return((uhel = (uhel > 0.0f) ? -(2*PI-uhel) : (2*PI+uhel)));
  }
  else {
    return(uhel);
  }
}

#define DELTA_MIN_ROZDIL 0.00000001f

#define stejny_vertex(v1,v2)       (fabsf(v1.x-v2.x) < DELTA_MIN_ROZDIL &&\
                                    fabsf(v1.y-v2.y) < DELTA_MIN_ROZDIL &&\
                                    fabsf(v1.z-v2.z) < DELTA_MIN_ROZDIL)\

__inline int stejny_vertex_point(BOD *v1, BOD *v2)
{
  return(fabsf(v1->x-v2->x) < DELTA_MIN_ROZDIL &&
         fabsf(v1->y-v2->y) < DELTA_MIN_ROZDIL &&
         fabsf(v1->z-v2->z) < DELTA_MIN_ROZDIL);
}

/* Funkce vcetne Delta-Planu
*/
__inline int stejny_vertex_point_delta(BOD *v1, BOD *v2, float delta)
{
  return(fabsf(v1->x-v2->x) < delta &&
         fabsf(v1->y-v2->y) < delta &&
         fabsf(v1->z-v2->z) < delta);
}


#endif
