//------------------------------------------------------------------------------------------------
// version 0.0.1
//------------------------------------------------------------------------------------------------
#include "..\\komat\\mss_on.h"
#include <math.h>
#include <direct.h>

#include <assert.h>
//#include <d3d.h>
#include <windows.h>
#include "font.h"

#include "..\komat\graphics.h"
#include "..\Komat\berusky3d_kofola_interface.h"
#include "..\komat\berusky3d_light.h"

#include "3D_graphic.h"
#include "Object.h"
#include "menu_script.h"
#include "controls.h"
#include "2d_graphic.h"
#include "menu2.h"
#include "apak.h"
#include "animation.h"
#include "3d_menus.h"
#include "load_level.h"
#include "menu_def.h"

extern ANIMATION_MODULE	am;
extern  AUDIO_DATA ad;
extern _2D_DATA	_2dd;
extern _3D_DATA	_3dd;
extern	G_KONFIG ber;
extern _3D_CURSOR _3dCur;
extern B2_FONT	b2_font;
extern int cameraflag;

HDC_INFO	hdc_info;

HINT_STATE	sHint;
KUK_STATE	sKuk;
EDIT_TEXT	sIndikace[3];

extern APAK_HANDLE		*pBmpArchive;
extern APAK_HANDLE		*p3DMArchive;
extern APAK_HANDLE		*pDataArchive;

extern	   HDC CompositDC;
extern	   HDC FontDC;
extern	   HDC BackDC;

typedef struct
{
	float	vfLocation[4];
	int		iTexture;
	int		iNextFrame;
	int		iTimeToNextFrame;
	char	cCleanDrawLine;
	float	fScale;
} _3D_ANIMATION_FRAME;

typedef struct __3D_ANIMATION
{
	_3D_ANIMATION_FRAME		_3dFrame[32];
	int						iLastFrame;
	int						iActFrame;
	int						iTimeCounter;
	char					bStop;
	int						iID;
	int						*pFlag;
	struct __3D_ANIMATION	*pNext;
	struct __3D_ANIMATION	*pPrevious;
} _3D_ANIMATION;

typedef struct __3D_DRAW_LIST
{
	float					vfLocation[4];
	int						iTexture;
	int						iOwnerID;
	struct __3D_DRAW_LIST	*pNext;
	struct __3D_DRAW_LIST	*pPrevious;
} _3D_DRAW_LIST;

typedef struct
{
	_3D_ANIMATION	_3DAnimation[32];
	_3D_DRAW_LIST	*pFDrawList;
	_3D_DRAW_LIST	*pLDrawList;
	_3D_ANIMATION	*pFAnimation;
	_3D_ANIMATION	*pLAnimation;
	int				iLastID;
	char			bMenuButton;
	int				iMenuButtonID;
	char			bItem;
	int				iInvIndex;
	int				iItemIndex;
	int				iItemID;
	int				iCibuleID;
	char			bLastMenuItem;
	int				iLastMenuItemID;
	int				iLastIsIn;
} _3D_ANIMATION_STRUCTURE;

_3D_ANIMATION_STRUCTURE	_3DAnimationStruct;
extern AUDIO_DATA		ad;
int	iTopLedgeDL;
int	iInventoryDL;
HINT_TEXTURE	pHintTexture[26];
HINT_TEXTURE	pMessageTexture[8];

#define HINT_SCALE	1.0f

//888888888888888888888888888888888888888888
//extern int	menu_texture;
//888888888888888888888888888888888888888888

void _3d_Stop_Animation(int iID, char cCleanDrawLine);
void _3d_RemoveIDFromDrawList(int iID);
void _3d_Obsluha_Game_Menu(char *bCursor, int *Cursor_Time_Out, LEVELINFO *p_Level,
						   float *_3d_Scale_Factor, int demo);
void RunMenuSettings3D(char *p_File_Name, HWND hWnd, AUDIO_DATA *p_ad, int cpu, HDC hdc, float *_3d_Scale_Factor);
void _3d_Start_Settings(float *_3d_Scale_Factor);
void _3d_Draw_Hint(float *_3d_Scale_Factor);
void _3d_Draw_Indikace(LEVELINFO *p_Level, float *_3d_Scale_Factor);
int gl_Inventory_Item_To_Object(LEVELINFO *p_Level, int Item, char bDec);

void _3d_CleanDrawList(void)
{
	_3D_DRAW_LIST	*pDrawList = _3DAnimationStruct.pFDrawList;
	_3D_DRAW_LIST	*pNDrawList;

	if(!pDrawList)
		return;

	_3DAnimationStruct.pLDrawList = NULL;

	while(pDrawList)
	{
		pNDrawList = pDrawList->pNext;
		free((void *) pDrawList);
		pDrawList = pNDrawList;
	}

	_3DAnimationStruct.pFDrawList = NULL;
}

void _3d_CleanAnimationList(void)
{
	_3D_ANIMATION	*pAnim = _3DAnimationStruct.pFAnimation;
	_3D_ANIMATION	*pNAnim;

	if(!pAnim)
		return;

	_3DAnimationStruct.pLAnimation = NULL;

	while(pAnim)
	{
		pNAnim = pAnim->pNext;
		
		if(pAnim->pFlag)
			*pAnim->pFlag = 1;

		_3d_RemoveIDFromDrawList(pAnim->iID);

		free((void *) pAnim);
		pAnim = pNAnim;
	}

	_3DAnimationStruct.pFAnimation = NULL;
}

void _3d_RemoveDrawListItem(_3D_DRAW_LIST *pItem)
{
	if(!pItem->pPrevious)
		_3DAnimationStruct.pFDrawList = pItem->pNext;
	else
		pItem->pPrevious->pNext = pItem->pNext;
			
	if(!pItem->pNext)
		_3DAnimationStruct.pLDrawList = pItem->pPrevious;
	else
		pItem->pNext->pPrevious = pItem->pPrevious;

	free((void *) pItem);
}

void _3d_AddItemToDrawList(_3D_ANIMATION_FRAME *pFrame, int iOwnerID)
{
	_3D_DRAW_LIST	*pLDrawList = _3DAnimationStruct.pLDrawList;
	_3D_DRAW_LIST	*pDrawList = (_3D_DRAW_LIST *)malloc(sizeof(_3D_DRAW_LIST));

	if(!pDrawList)
	{
		kprintf(1,"Neporilo se alokovat pamet pro 'DrawListItem' iOwnerID: %d", iOwnerID);
		return;
	}

	if(!pLDrawList)
	{
		_3DAnimationStruct.pFDrawList = pDrawList;
		_3DAnimationStruct.pLDrawList = pDrawList;
		pDrawList->pPrevious = NULL;
	}
	else
	{
		pDrawList->pPrevious = pLDrawList;
		pLDrawList->pNext = pDrawList;
	}

//	kprintf(1, "%d - pridano do draw listu ....", pFrame->iTexture);

	pDrawList->iOwnerID = iOwnerID;
	pDrawList->iTexture = pFrame->iTexture;
	memcpy((void *)pDrawList->vfLocation, (void *) pFrame->vfLocation, 4*sizeof(float));
	pDrawList->pNext = NULL;
	_3DAnimationStruct.pLDrawList = pDrawList;
}

void _3d_RemoveIDFromDrawList(int iID)
{
	_3D_DRAW_LIST *pItem = _3DAnimationStruct.pFDrawList;
	_3D_DRAW_LIST *pNextItem;

	while(pItem)
	{
		pNextItem = pItem->pNext;

		if(pItem->iOwnerID == iID)
			_3d_RemoveDrawListItem(pItem);

		pItem = pNextItem;
	}
}

//------------------------------------------------------------------------------------------------
// provede animaci
//------------------------------------------------------------------------------------------------
void _3d_Animation(_3D_ANIMATION *p3DAnim, int iTime)
{
_3D_ANIMATION_DRAW:

	if(iTime >= 0)
	{
		if(p3DAnim->_3dFrame[p3DAnim->iActFrame].cCleanDrawLine)
			_3d_RemoveIDFromDrawList(p3DAnim->iID);

		if(p3DAnim->_3dFrame[p3DAnim->iActFrame].iNextFrame == -1)
		{
			p3DAnim->bStop = 1;
			return;
		}

		iTime -= p3DAnim->_3dFrame[p3DAnim->iActFrame].iTimeToNextFrame;

		p3DAnim->iActFrame = p3DAnim->_3dFrame[p3DAnim->iActFrame].iNextFrame;
	}

	while(iTime > 0)
	{
		if(p3DAnim->_3dFrame[p3DAnim->iActFrame].iNextFrame == -1)
		{
			_3d_AddItemToDrawList(&p3DAnim->_3dFrame[p3DAnim->iActFrame], p3DAnim->iID);
			p3DAnim->bStop = 1;
			break;
		}

		iTime -= p3DAnim->_3dFrame[p3DAnim->iActFrame].iTimeToNextFrame;

		if(iTime > (p3DAnim->_3dFrame[p3DAnim->_3dFrame[p3DAnim->iActFrame].iNextFrame].iTimeToNextFrame / 2.0f))
		{
			_3d_AddItemToDrawList(&p3DAnim->_3dFrame[p3DAnim->iActFrame], p3DAnim->iID);
			p3DAnim->iActFrame = p3DAnim->_3dFrame[p3DAnim->iActFrame].iNextFrame;
		}
		else
			if(!p3DAnim->_3dFrame[p3DAnim->iActFrame].iTimeToNextFrame)
				break;
	}

	iTime = 0;

	_3d_AddItemToDrawList(&p3DAnim->_3dFrame[p3DAnim->iActFrame], p3DAnim->iID);

	if(!p3DAnim->_3dFrame[p3DAnim->iActFrame].iTimeToNextFrame)
		goto _3D_ANIMATION_DRAW;

	if(p3DAnim->_3dFrame[p3DAnim->iActFrame].iNextFrame == -1)
	{
		_3d_AddItemToDrawList(&p3DAnim->_3dFrame[p3DAnim->iActFrame], p3DAnim->iID);
		p3DAnim->bStop = 1;
		return;
	}
}

int _3d_Start_Animation(int iIndex, float *vfLocation, int *pFlag)
{
	int i;

	_3D_ANIMATION	*pL3DAnim = _3DAnimationStruct.pLAnimation;
	_3D_ANIMATION	*p3DAnim = (_3D_ANIMATION *)malloc(sizeof(_3D_ANIMATION));

	if(!p3DAnim)
	{
		kprintf(1,"Neporilo se alokovat pamet pro 3D animaci: %d", iIndex);
		return -1;
	}

	memcpy((void *) p3DAnim, (void *) &_3DAnimationStruct._3DAnimation[iIndex], sizeof(_3D_ANIMATION));

	if(!pL3DAnim)
	{
		_3DAnimationStruct.pFAnimation = p3DAnim;
		_3DAnimationStruct.pLAnimation = p3DAnim;
		p3DAnim->pPrevious = NULL;
	}
	else
	{
		p3DAnim->pPrevious = pL3DAnim;
		pL3DAnim->pNext = p3DAnim;
	}
	
	p3DAnim->bStop = 0;
	p3DAnim->iID = _3DAnimationStruct.iLastID;
	_3DAnimationStruct.iLastID++;
	p3DAnim->iActFrame = 0;
	p3DAnim->iTimeCounter = 0;
	p3DAnim->pNext = NULL;
	p3DAnim->pFlag = pFlag;
	_3DAnimationStruct.pLAnimation = p3DAnim;

	if(vfLocation)
		for(i=0;i<p3DAnim->iLastFrame;i++)
			memcpy((void *)p3DAnim->_3dFrame[i].vfLocation, (void *)vfLocation, 4 * sizeof(float));

	_3d_Animation(p3DAnim, -1);

	return 1;
}

void _3d_Stop_Animation(int iID, char cCleanDrawLine)
{
	_3D_ANIMATION	*p3DAnim = _3DAnimationStruct.pFAnimation;

	if(!p3DAnim)
		return;

	while(p3DAnim)
	{
		if(p3DAnim->iID == iID)
		{
			if(cCleanDrawLine)
				_3d_RemoveIDFromDrawList(iID);

			if(!p3DAnim->pPrevious)
				_3DAnimationStruct.pFAnimation = p3DAnim->pNext;
			else
				p3DAnim->pPrevious->pNext = p3DAnim->pNext;
			
			if(!p3DAnim->pNext)
				_3DAnimationStruct.pLAnimation = p3DAnim->pPrevious;
			else
				p3DAnim->pNext->pPrevious = p3DAnim->pPrevious;

			if(p3DAnim->pFlag)
				*p3DAnim->pFlag = 1;

			free((void *) p3DAnim);

			return;
		}

		p3DAnim = p3DAnim->pNext;
	}
}

void _3d_Draw_3DAnimations(void)
{
	_3D_DRAW_LIST *pItem = _3DAnimationStruct.pFDrawList;

	if(!pItem)
		return;

	while(pItem)
	{
		_3d_Draw_Box(pItem->iTexture, pItem->vfLocation);
		pItem = pItem->pNext;
	}
}

//------------------------------------------------------------------------------------------------
// provede animace
//------------------------------------------------------------------------------------------------
void _3d_AnimationEvent(void)
{
	_3D_ANIMATION_STRUCTURE	*p3DAnimationStruct = &_3DAnimationStruct;
	_3D_ANIMATION	*p3DAnim = p3DAnimationStruct->pFAnimation;
	_3D_ANIMATION	*p3DNAnim;
	DWORD e = ber.TimeLastFrame;

	while(p3DAnim)
	{
		p3DAnim->iTimeCounter += e;

		p3DNAnim = p3DAnim->pNext;

		if(p3DAnim->bStop)
		{
			_3d_Stop_Animation(p3DAnim->iID, p3DAnim->_3dFrame[p3DAnim->iLastFrame-1].cCleanDrawLine);
		}
		else
		{
			if(p3DAnim->iTimeCounter > p3DAnim->_3dFrame[p3DAnim->iActFrame].iTimeToNextFrame)
			{
				_3d_Animation(p3DAnim, p3DAnim->iTimeCounter);
				p3DAnim->iTimeCounter = 0;
			}
		}

		p3DAnim = p3DNAnim;
	}
}

//------------------------------------------------------------------------------------------------
// naparsuje radek animace
//------------------------------------------------------------------------------------------------
void Parse_3DAnimLine(FILE *file, _3D_ANIMATION_FRAME *pFrame)
{
	char	text[256], expression[256];
	int		p = 0, r = 0;
	
	agets(text, 256, file);

	while(p != -1)
	{
		p = Find_Next_Expresion(text, p, expression);
		
		if(p != -1)
			switch(r)
			{
				case 1:
					pFrame->iTexture = atoi(expression);
					break;
				case 2:
				case 3:
				case 4:
				case 5:
					pFrame->vfLocation[r-2] = (float)atof(expression);
					break;
				case 6:
					pFrame->iTimeToNextFrame = atoi(expression);
					break;
				case 7:
					pFrame->iNextFrame = atoi(expression);
					break;
				case 8:
					pFrame->cCleanDrawLine = (char)atoi(expression);
					break;
				case 9:
					pFrame->fScale = (float)atof(expression);
					break;
			}

		r++;
	}
}

void _3d_Add_To_OpenMenuAnim(_3D_ANIMATION	*p3DAnim)
{
	int i;

	p3DAnim->_3dFrame[p3DAnim->iLastFrame-1].iNextFrame = p3DAnim->iLastFrame;

	for(i=16;i<22;i++)
	{
		p3DAnim->_3dFrame[p3DAnim->iLastFrame].cCleanDrawLine = 0;
		p3DAnim->_3dFrame[p3DAnim->iLastFrame].fScale = _3DAnimationStruct._3DAnimation[i]._3dFrame[0].fScale;
		p3DAnim->_3dFrame[p3DAnim->iLastFrame].iNextFrame = p3DAnim->iLastFrame + 1;
		p3DAnim->_3dFrame[p3DAnim->iLastFrame].iTexture = _3DAnimationStruct._3DAnimation[i]._3dFrame[0].iTexture;
		p3DAnim->_3dFrame[p3DAnim->iLastFrame].iTimeToNextFrame = 0;
		p3DAnim->_3dFrame[p3DAnim->iLastFrame].vfLocation[0] = _3DAnimationStruct._3DAnimation[i]._3dFrame[0].vfLocation[0];
		p3DAnim->_3dFrame[p3DAnim->iLastFrame].vfLocation[1] = _3DAnimationStruct._3DAnimation[i]._3dFrame[0].vfLocation[1];
		p3DAnim->_3dFrame[p3DAnim->iLastFrame].vfLocation[2] = _3DAnimationStruct._3DAnimation[i]._3dFrame[0].vfLocation[2];
		p3DAnim->_3dFrame[p3DAnim->iLastFrame].vfLocation[3] = _3DAnimationStruct._3DAnimation[i]._3dFrame[0].vfLocation[3];
		p3DAnim->iLastFrame++;
	}

	p3DAnim->_3dFrame[p3DAnim->iLastFrame].cCleanDrawLine = 0;
	p3DAnim->_3dFrame[p3DAnim->iLastFrame].fScale = _3DAnimationStruct._3DAnimation[i]._3dFrame[0].fScale;
	p3DAnim->_3dFrame[p3DAnim->iLastFrame].iNextFrame = -1;
	p3DAnim->_3dFrame[p3DAnim->iLastFrame].iTexture = _3DAnimationStruct._3DAnimation[i]._3dFrame[0].iTexture;
	p3DAnim->_3dFrame[p3DAnim->iLastFrame].iTimeToNextFrame = 50;
	p3DAnim->_3dFrame[p3DAnim->iLastFrame].vfLocation[0] = _3DAnimationStruct._3DAnimation[i]._3dFrame[0].vfLocation[0];
	p3DAnim->_3dFrame[p3DAnim->iLastFrame].vfLocation[1] = _3DAnimationStruct._3DAnimation[i]._3dFrame[0].vfLocation[1];
	p3DAnim->_3dFrame[p3DAnim->iLastFrame].vfLocation[2] = _3DAnimationStruct._3DAnimation[i]._3dFrame[0].vfLocation[2];
	p3DAnim->_3dFrame[p3DAnim->iLastFrame].vfLocation[3] = _3DAnimationStruct._3DAnimation[i]._3dFrame[0].vfLocation[3];
	p3DAnim->iLastFrame++;

}

void _3d_Load_Animations(float *fScaleFactor)
{
	_3D_ANIMATION_FRAME	*pFrame;
	_3D_ANIMATION	*p3DAnim;
	FILE	*pFile = NULL, *pListFile = NULL;
	char	text[256];
	int		i = 0, j;

	GetPrivateProfileString("game","data_dir","c:\\",text,256,ini_file);
	achdir(pDataArchive, text);

	pListFile = aopen(pDataArchive, "3DAnimations.dat","r");

	if(pListFile)
	{
		while(!aeof(pListFile))
		{
			agets(text,256,pListFile);
			
			if(!strlen(text))
				break;
			else
				text[strlen(text)-1] = '\0';

			pFile = aopen(pDataArchive, text,"r");

			if(pFile)
			{
				p3DAnim = &_3DAnimationStruct._3DAnimation[i];
				j = 0;

				while(!aeof(pFile))
				{
					pFrame = &p3DAnim->_3dFrame[j];

					Parse_3DAnimLine(pFile, pFrame);
					
					if(i >= 16 && i < 23)
					{
						float dx;

						pFrame->iTexture += MENU_ANIMATION_CORECTION;

						dx = ((pFrame->vfLocation[2] - pFrame->vfLocation[0]) - (_3dd.p_sysramtexture[pFrame->iTexture].tx * pFrame->fScale)) / 2.0f;

						pFrame->vfLocation[0] += dx;
						pFrame->vfLocation[2] = pFrame->vfLocation[0] + (_3dd.p_sysramtexture[pFrame->iTexture].x * pFrame->fScale);
						pFrame->vfLocation[3] = pFrame->vfLocation[1] + _3dd.p_sysramtexture[pFrame->iTexture].y * pFrame->fScale;

						pFrame->vfLocation[0] *= fScaleFactor[0];
						pFrame->vfLocation[1] *= fScaleFactor[1];
						pFrame->vfLocation[2] *= fScaleFactor[0];
						pFrame->vfLocation[3] *= fScaleFactor[1];
					}
					else
					{
						pFrame->vfLocation[0] *= fScaleFactor[0];
						pFrame->vfLocation[1] *= fScaleFactor[1];
						pFrame->vfLocation[2] *= fScaleFactor[0];
						pFrame->vfLocation[3] *= fScaleFactor[1];
					}

					j++;
				}

				p3DAnim->bStop = 0;
				p3DAnim->iActFrame = 0;
				p3DAnim->iLastFrame = j - 1;
				p3DAnim->iID = i;
				
				i++;
				aclose(pFile);

				strcpy(text,"");
			}
			else
				kprintf(1,"Nenalezen 3danimacni soubor %s", text);
		}

		_3d_Add_To_OpenMenuAnim(&_3DAnimationStruct._3DAnimation[14]);

		aclose(pListFile);
	}
	else
	{
		//MessageBox(NULL,"Nenalezen soubor se seznamem 3d animaci!","Chyba", MB_OK);
		kprintf(1,"Nenalezen soubor se seznamem 3d animaci!");
	}

	_3DAnimationStruct.pFDrawList = NULL;
	_3DAnimationStruct.pLDrawList = NULL;
	_3DAnimationStruct.pFAnimation = NULL;
	_3DAnimationStruct.pLAnimation = NULL;
	_3DAnimationStruct.bMenuButton = 0;
	_3DAnimationStruct.bItem = 0;
	_3DAnimationStruct.iItemID = -1;
	_3DAnimationStruct.iItemIndex = -1;
	_3DAnimationStruct.iInvIndex = -1;
	_3DAnimationStruct.iCibuleID = -1;
	_3DAnimationStruct.bLastMenuItem = 0;
	_3DAnimationStruct.iLastMenuItemID = -1;
	_3DAnimationStruct.iLastIsIn = -1;
}

void _3d_Draw_Item(float *pos, int iBackPack, int iItem, LEVELINFO *p_Level, char bAnimate)
{
	if(p_Level->Actual_Item == -1)
		return;

	if(bAnimate)
	{
		if(!iBackPack)
			iBackPack += p_Level->Level[p_Level->Actual_Item]->p_Object->SubClass;
		else
			iBackPack += 4;

		if(_3DAnimationStruct.bItem &&
		   _3DAnimationStruct.iItemIndex == iBackPack &&
		   _3DAnimationStruct.iInvIndex == iItem)
			return;
		else
			if(_3DAnimationStruct.bItem &&
			   (_3DAnimationStruct.iItemIndex != iBackPack || 
			    _3DAnimationStruct.iInvIndex != iItem))
					_3d_Stop_Animation(_3DAnimationStruct.iItemID, 1);

		_3d_Start_Animation(iBackPack + 1, pos, NULL);
		_3DAnimationStruct.iItemID = _3DAnimationStruct.iLastID - 1;
		_3DAnimationStruct.iItemIndex = iBackPack;
		_3DAnimationStruct.iInvIndex = iItem;
		_3DAnimationStruct.bItem = 1;
	}
	else
		switch(iBackPack)
		{
		case 0:
			_3d_Draw_Box(26 + (6*p_Level->Level[p_Level->Actual_Item]->p_Object->SubClass), pos);
			break;
		case 4:
			_3d_Draw_Box(56, pos);
			break;
		case 7:
			_3d_Draw_Box(68, pos);
			break;
		case 8:
			_3d_Draw_Box(62, pos);
			break;
		}
}

void _3d_Animate_Item(float *pos, int iItem, LEVELINFO *p_Level)
{
	int m = 0;
	int i;

	for(i = 0; i<16; i++)
	{
		if(i != 3 && i != 5)
			m += p_Level->Level[p_Level->Actual_Item]->p_Back_Pack->item[i];

		if( m > iItem)
			break;

	}

	_3d_Draw_Item(pos, i, iItem, p_Level, 1);
}

//------------------------------------------------------------------------------------------------
// kresli inventorar
//------------------------------------------------------------------------------------------------
int _3d_Draw_Invertory(LEVELINFO *p_Level, int act_item, float	*_3d_Scale_Factor, char bCursor)
{
	char bAnimated = 0;
	float pos[4];
	int ret = -1,i,c,m = 0;

	glCallList(iInventoryDL);

	if(p_Level->Actual_Item == -1)
		return ret;

	for(i=0;i<16;i++)
	if(i != 3 && i != 5)
		for(c=0;c<p_Level->Level[p_Level->Actual_Item]->p_Back_Pack->item[i];c++)
		{
			switch(m)
			{
			case 0:
				pos[0] = 68 * _3d_Scale_Factor[0];
				pos[1] = 696 * _3d_Scale_Factor[1];
				pos[2] = 100 * _3d_Scale_Factor[0];
				pos[3] = 728 * _3d_Scale_Factor[1];
				_3d_Draw_Item(pos, i, act_item, p_Level, 0);
				m++;
				break;
			case 1:
				pos[0] = 114 * _3d_Scale_Factor[0];
				pos[1] = 726 * _3d_Scale_Factor[1];
				pos[2] = 146 * _3d_Scale_Factor[0];
				pos[3] = 758 * _3d_Scale_Factor[1];
				_3d_Draw_Item(pos, i, act_item, p_Level, 0);
				m++;
				break;
			case 2:
				pos[0] = 167 * _3d_Scale_Factor[0];
				pos[1] = 726 * _3d_Scale_Factor[1];
				pos[2] = 199 * _3d_Scale_Factor[0];
				pos[3] = 758 * _3d_Scale_Factor[1];
				_3d_Draw_Item(pos, i, act_item, p_Level, 0);
				m++;
				break;
			case 3:
				pos[0] = 221 * _3d_Scale_Factor[0];
				pos[1] = 726 * _3d_Scale_Factor[1];
				pos[2] = 254 * _3d_Scale_Factor[0];
				pos[3] = 758 * _3d_Scale_Factor[1];
				_3d_Draw_Item(pos, i, act_item, p_Level, 0);
				m++;
				break;
			}
		}
	
	m = 0;
	
	if(bCursor)
	{
		for(i=0;i<16;i++)
		if(i != 3 && i != 5)
			m+=p_Level->Level[p_Level->Actual_Item]->p_Back_Pack->item[i];

		if(m > 0)
		if((mi.x >= 67 * _3d_Scale_Factor[0]) && (mi.x <= 101 * _3d_Scale_Factor[0]) &&
		   (mi.y >= 696 * _3d_Scale_Factor[1]) && (mi.y <= 728 * _3d_Scale_Factor[1]))
		{
			bAnimated = 1;
			pos[0] = 68 * _3d_Scale_Factor[0];
			pos[1] = 696 * _3d_Scale_Factor[1];
			pos[2] = 100 * _3d_Scale_Factor[0];
			pos[3] = 728 * _3d_Scale_Factor[1];
			_3d_Animate_Item(pos, 0, p_Level);
			if(mi.t1)
				ret = 0;

			_3d_Display_Hint_Inventory(p_Level, p_Level->Level[p_Level->Actual_Item], 0, 68, 696, _3d_Scale_Factor);
		}

		if(m > 1)
		if((mi.x >= 114 * _3d_Scale_Factor[0]) && (mi.x <= 146 * _3d_Scale_Factor[0]) &&
		   (mi.y >= 725 * _3d_Scale_Factor[1]) && (mi.y <= 759 * _3d_Scale_Factor[1]))
		{
			bAnimated = 1;
			pos[0] = 114 * _3d_Scale_Factor[0];
			pos[1] = 726 * _3d_Scale_Factor[1];
			pos[2] = 146 * _3d_Scale_Factor[0];
			pos[3] = 758 * _3d_Scale_Factor[1];
			_3d_Animate_Item(pos, 1, p_Level);
			if(mi.t1)
				ret = 1;

			_3d_Display_Hint_Inventory(p_Level, p_Level->Level[p_Level->Actual_Item], 1, 114, 726, _3d_Scale_Factor);
		}
		
		if(m > 2)
		if((mi.x >= 167 * _3d_Scale_Factor[0]) && (mi.x <= 199 * _3d_Scale_Factor[0]) &&
		   (mi.y >= 725 * _3d_Scale_Factor[1]) && (mi.y <= 759 * _3d_Scale_Factor[1]))
		{
			bAnimated = 1;
			pos[0] = 167 * _3d_Scale_Factor[0];
			pos[1] = 726 * _3d_Scale_Factor[1];
			pos[2] = 199 * _3d_Scale_Factor[0];
			pos[3] = 758 * _3d_Scale_Factor[1];
			_3d_Animate_Item(pos, 2, p_Level);
			if(mi.t1)
				ret = 2;

			_3d_Display_Hint_Inventory(p_Level, p_Level->Level[p_Level->Actual_Item], 2, 167, 726, _3d_Scale_Factor);
		}
		
		if(m > 3)
		if((mi.x >= 221 * _3d_Scale_Factor[0]) && (mi.x <= 254 * _3d_Scale_Factor[0]) &&
		   (mi.y >= 725 * _3d_Scale_Factor[1]) && (mi.y <= 759 * _3d_Scale_Factor[1]))
		{
			bAnimated = 1;
			pos[0] = 221 * _3d_Scale_Factor[0];
			pos[1] = 726 * _3d_Scale_Factor[1];
			pos[2] = 254 * _3d_Scale_Factor[0];
			pos[3] = 758 * _3d_Scale_Factor[1];
			_3d_Animate_Item(pos, 3, p_Level);
			if(mi.t1)
				ret = 3;

			_3d_Display_Hint_Inventory(p_Level, p_Level->Level[p_Level->Actual_Item], 3, 221, 726, _3d_Scale_Factor);
		}
	}
	else
	{
		if(!act_item)
		{
			bAnimated = 1;
			pos[0] = 68 * _3d_Scale_Factor[0];
			pos[1] = 696 * _3d_Scale_Factor[1];
			pos[2] = 100 * _3d_Scale_Factor[0];
			pos[3] = 728 * _3d_Scale_Factor[1];
			_3d_Animate_Item(pos, 0, p_Level);
		}

		if(act_item == 1)	
		{
			bAnimated = 1;
			pos[0] = 114 * _3d_Scale_Factor[0];
			pos[1] = 726 * _3d_Scale_Factor[1];
			pos[2] = 146 * _3d_Scale_Factor[0];
			pos[3] = 758 * _3d_Scale_Factor[1];
			_3d_Animate_Item(pos, 1, p_Level);
		}
		
		if(act_item == 2)
		{
			bAnimated = 1;
			pos[0] = 167 * _3d_Scale_Factor[0];
			pos[1] = 726 * _3d_Scale_Factor[1];
			pos[2] = 199 * _3d_Scale_Factor[0];
			pos[3] = 758 * _3d_Scale_Factor[1];
			_3d_Animate_Item(pos, 2, p_Level);
		}
		
		if(act_item == 3)
		{
			bAnimated = 1;
			pos[0] = 221 * _3d_Scale_Factor[0];
			pos[1] = 726 * _3d_Scale_Factor[1];
			pos[2] = 254 * _3d_Scale_Factor[0];
			pos[3] = 758 * _3d_Scale_Factor[1];
			_3d_Animate_Item(pos, 3, p_Level);
		}
	}

	if(!bAnimated && _3DAnimationStruct.bItem)
	{
		_3DAnimationStruct.bItem = 0;
		_3d_Stop_Animation(_3DAnimationStruct.iItemID, 1);
		_3DAnimationStruct.iItemID = -1;
		_3DAnimationStruct.iItemIndex = -1;
		sHint.bHint = 0;
	}

	//kprintf(1, "ret %d", ret);
	return ret;
}


//------------------------------------------------------------------------------------------------
// spocte vzdalenost dvou bodu
//------------------------------------------------------------------------------------------------
float _3d_Count_Distance(float *a, float *b)
{
  return((float)sqrt((b[0]-a[0])*(b[0]-a[0])+(b[1]-a[1])*(b[1]-a[1])));
}

void _3d_Stop_Menu_Select(void)
{
	if(_3DAnimationStruct.bMenuButton)
	{
		_3DAnimationStruct.bMenuButton = 0;
		_3d_Stop_Animation(_3DAnimationStruct.iMenuButtonID, 1);
	}
}

int _3d_Check_Menu_Select(float *_3d_Scale_Factor, char bCursor, LEVELINFO *p_Level)
{
	if(p_Level->bMenuRunning)
		return 0;

	if((mi.x >= 960 * _3d_Scale_Factor[0]) && (mi.x <= 1019 * _3d_Scale_Factor[0]) &&
	   (mi.y >= 95 * _3d_Scale_Factor[1]) && (mi.y <= 111 * _3d_Scale_Factor[1]) &&
	   bCursor)
	{
		if(!_3DAnimationStruct.bMenuButton)
		{
			_3DAnimationStruct.bMenuButton = 1;
			_3d_Start_Animation(0, NULL, NULL);
			_3DAnimationStruct.iMenuButtonID = _3DAnimationStruct.iLastID - 1;
		}

		return 1;
	}
	else
	{
		if(_3DAnimationStruct.bMenuButton)
		{
			_3DAnimationStruct.bMenuButton = 0;
			_3d_Stop_Animation(_3DAnimationStruct.iMenuButtonID, 1);
		}

		return 0;
	}
}

//------------------------------------------------------------------------------------------------
// zkontroluje, zda je oznacen nejakej brouk
//------------------------------------------------------------------------------------------------
int _3d_Check_Beatle_Select(float *_3d_Scale_Factor)
{
	float c[2],m[2];

	m[0] = (float)mi.x;
	m[1] = (float)mi.y;

	c[0] = 194 * _3d_Scale_Factor[0];
	c[1] = 57 * _3d_Scale_Factor[1];
	if(_3d_Count_Distance(c,m) <= 32 * _3d_Scale_Factor[0])
		return 0;

	c[0] = 279 * _3d_Scale_Factor[0];
	c[1] = 58 * _3d_Scale_Factor[1];
	if(_3d_Count_Distance(c,m) <= 32 * _3d_Scale_Factor[0])
		return 1;

	c[0] = 439 * _3d_Scale_Factor[0];
	c[1] = 88 * _3d_Scale_Factor[1];
	if(_3d_Count_Distance(c,m) <= 32 * _3d_Scale_Factor[0])
		return 2;

	c[0] = 529 * _3d_Scale_Factor[0];
	c[1] = 89 * _3d_Scale_Factor[1];
	if(_3d_Count_Distance(c,m) <= 32 * _3d_Scale_Factor[0])
		return 3;

	c[0] = 617 * _3d_Scale_Factor[0];
	c[1] = 88 * _3d_Scale_Factor[1];
	if(_3d_Count_Distance(c,m) <= 32 * _3d_Scale_Factor[0])
		return 4;

	c[0] = 777 * _3d_Scale_Factor[0];
	c[1] = 74 * _3d_Scale_Factor[1];
	if(_3d_Count_Distance(c,m) <= 32 * _3d_Scale_Factor[0])
		return 5;

	return -1;
}

int _3d_Is_There_Beetle(int SubClass, LEVELINFO *p_Level)
{
	int i;

	for(i=0;i<6;i++)
		if(p_Level->Beetle[i] == SubClass)
			return i;

	return -1;
}

int _3d_Is_Selected(int SubClass, LEVELINFO *p_Level)
{
	if(p_Level->Beetle[p_Level->Beetle_Index] == SubClass)
		return 1;
	else
		return 0;
}

//------------------------------------------------------------------------------------------------
// draws top ledge
//------------------------------------------------------------------------------------------------
void _3d_Draw_Top_Ledge(LEVELINFO *p_Level, float *_3d_Scale_Factor, char bCursor)
{
	float cx, cy;
	float pos[4];

	cx = (float)mi.x + _3dCur.iaddx;
	cy = (float)mi.y + _3dCur.iaddy;

	//zluta beruska
	pos[0] = 162 * _3d_Scale_Factor[0];
	pos[1] = 25 * _3d_Scale_Factor[1];
	pos[2] = 226 * _3d_Scale_Factor[0];
	pos[3] = 89 * _3d_Scale_Factor[1];
	if(_3d_Is_Selected(0, p_Level))
		_3d_Draw_Box(6, pos);
	else
		if(_3d_Is_There_Beetle(0, p_Level) == -1)
			_3d_Draw_Box(4, pos);
		else
			_3d_Draw_Box(5, pos);

	//hneda beruska
	pos[0] = 247 * _3d_Scale_Factor[0];
	pos[1] = 26 * _3d_Scale_Factor[1];
	pos[2] = 311 * _3d_Scale_Factor[0];
	pos[3] = 90 * _3d_Scale_Factor[1];
	if(_3d_Is_Selected(1, p_Level))
		_3d_Draw_Box(9, pos);
	else
		if(_3d_Is_There_Beetle(1, p_Level) == -1)
			_3d_Draw_Box(7, pos);
		else
			_3d_Draw_Box(8, pos);

	//cervena beruska
	pos[0] = 407 * _3d_Scale_Factor[0];
	pos[1] = 56 * _3d_Scale_Factor[1];
	pos[2] = 471 * _3d_Scale_Factor[0];
	pos[3] = 120 * _3d_Scale_Factor[1];
	if(_3d_Is_Selected(2, p_Level))
		_3d_Draw_Box(12, pos);
	else
		if(_3d_Is_There_Beetle(2, p_Level) == -1)
			_3d_Draw_Box(10, pos);
		else
			_3d_Draw_Box(11, pos);

	//modra beruska
	pos[0] = 497 * _3d_Scale_Factor[0];
	pos[1] = 57 * _3d_Scale_Factor[1];
	pos[2] = 561 * _3d_Scale_Factor[0];
	pos[3] = 121 * _3d_Scale_Factor[1];
	if(_3d_Is_Selected(3, p_Level))
		_3d_Draw_Box(15, pos);
	else
		if(_3d_Is_There_Beetle(3, p_Level) == -1)
			_3d_Draw_Box(13, pos);
		else
			_3d_Draw_Box(14, pos);

	//zelena beruska
	pos[0] = 585 * _3d_Scale_Factor[0];
	pos[1] = 56 * _3d_Scale_Factor[1];
	pos[2] = 649 * _3d_Scale_Factor[0];
	pos[3] = 120 * _3d_Scale_Factor[1];
	if(_3d_Is_Selected(4, p_Level))
		_3d_Draw_Box(18, pos);
	else
		if(_3d_Is_There_Beetle(4, p_Level) == -1)
			_3d_Draw_Box(16, pos);
		else
			_3d_Draw_Box(17, pos);

	//vodomerka beruska
	pos[0] = 745 * _3d_Scale_Factor[0];
	pos[1] = 42 * _3d_Scale_Factor[1];
	pos[2] = 809 * _3d_Scale_Factor[0];
	pos[3] = 106 * _3d_Scale_Factor[1];

	if(cx > pos[0] && cx < pos[2] && cy > pos[1] && cy < pos[3] && !p_Level->bMenuRunning)
		_3d_Draw_Box(21, pos);
	else
		_3d_Draw_Box(20, pos);

/*	if(_3d_Is_Selected(5, p_Level))
		_3d_Draw_Box(21, pos);
	else
		if(_3d_Is_There_Beetle(5, p_Level) == -1)
			_3d_Draw_Box(19, pos);
		else
			_3d_Draw_Box(20, pos);*/

}

void _3d_Create_Top_Ledge_Display_List(float *_3d_Scale_Factor)
{
	float	pos[4];
	iTopLedgeDL = glGenLists(1);

	glNewList(iTopLedgeDL, GL_COMPILE);

	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 1024 * _3d_Scale_Factor[0];
	pos[3] = 128 * _3d_Scale_Factor[1];
	_3d_Draw_Box(1, pos);

	pos[0] = 0;
	pos[1] = 128 * _3d_Scale_Factor[1];
	pos[2] = 1024 * _3d_Scale_Factor[0];
	pos[3] = 192 * _3d_Scale_Factor[1];
	_3d_Draw_Box(2, pos);

	pos[0] = 896 * _3d_Scale_Factor[0];
	pos[1] = 192 * _3d_Scale_Factor[1];
	pos[2] = 1024 * _3d_Scale_Factor[0];
	pos[3] = 320 * _3d_Scale_Factor[1];
	_3d_Draw_Box(3, pos);

	pos[0] = 960 * _3d_Scale_Factor[0];
	pos[1] = 320 * _3d_Scale_Factor[1];
	pos[2] = 1024 * _3d_Scale_Factor[0];
	pos[3] = 352 * _3d_Scale_Factor[1];
	_3d_Draw_Box(104, pos);

	glEndList();
}

void _3d_Create_Inventory_Display_List(float *_3d_Scale_Factor)
{
	float	pos[4];
	
	iInventoryDL = glGenLists(1);

	glNewList(iInventoryDL, GL_COMPILE);

	pos[0] = 0;
	pos[1] = 704 * _3d_Scale_Factor[1];
	pos[2] = 256 * _3d_Scale_Factor[0];
	pos[3] = 768 * _3d_Scale_Factor[1];
	_3d_Draw_Box(22, pos);

	pos[0] = 256 * _3d_Scale_Factor[0];
	pos[1] = 704 * _3d_Scale_Factor[1];
	pos[2] = 288 * _3d_Scale_Factor[0];
	pos[3] = 768 * _3d_Scale_Factor[1];
	_3d_Draw_Box(105, pos);

	pos[0] = 0;
	pos[1] = 671 * _3d_Scale_Factor[1];
	pos[2] = 128 * _3d_Scale_Factor[0];
	pos[3] = 704 * _3d_Scale_Factor[1];
	_3d_Draw_Box(23, pos);

	pos[0] = 128 * _3d_Scale_Factor[0];
	pos[1] = 671 * _3d_Scale_Factor[1];
	pos[2] = 160 * _3d_Scale_Factor[0];
	pos[3] = 704 * _3d_Scale_Factor[1];
	_3d_Draw_Box(24, pos);

	pos[0] = 0;
	pos[1] = 607 * _3d_Scale_Factor[1];
	pos[2] = 32 * _3d_Scale_Factor[0];
	pos[3] = 671 * _3d_Scale_Factor[1];
	_3d_Draw_Box(106, pos);

	glEndList();
}

void _3d_Release_Display_Lists(void)
{
	glDeleteLists(iTopLedgeDL, 1);
	glDeleteLists(iInventoryDL, 1);
}

void _3d_Draw_MenusB(LEVELINFO *p_Level, int act_item, char *bCursor, float *_3d_Scale_Factor, int bTutor, int bTText, int ty)
{
	float pos[4];

	if(p_Level->bTopLedge)
	{
		glCallList(iTopLedgeDL);
		_3d_Draw_Top_Ledge(p_Level, _3d_Scale_Factor, *bCursor);
	}

	if(p_Level->bInventory)
		_3d_Draw_Invertory(p_Level, act_item, _3d_Scale_Factor, *bCursor);

	_3d_AnimationEvent();
	_3d_Draw_3DAnimations();

	if(bTutor)
	{
		_3d_Set_Smooth();
		pos[0] = 10 * _3d_Scale_Factor[0];
		pos[1] = 200 * _3d_Scale_Factor[1];
		pos[2] = 1034 * _3d_Scale_Factor[0];
		pos[3] = (ty * _3d_Scale_Factor[1]);

		/*pos[0] = 10 * _3d_Scale_Factor[0];
		pos[1] = 200 * _3d_Scale_Factor[1];
		pos[2] = 300 * _3d_Scale_Factor[0];
		pos[3] = 650 * _3d_Scale_Factor[1];*/

		glBindTexture(GL_TEXTURE_2D, bTText);

		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(0,1);
		glVertex2d(pos[0], pos[1]);
		glTexCoord2d(0,0);
		glVertex2d(pos[0], pos[3]);
		glTexCoord2d(1,1);
		glVertex2d(pos[2], pos[1]);
		glTexCoord2d(1,0);
		glVertex2d(pos[2], pos[3]);
		glEnd();
		_3d_Set_Mask();
	}
}

void _3d_Start_Kuk(float *_3d_Scale_Factor)
{
	sKuk.y = randf() * (640 * _3d_Scale_Factor[1]);
	sKuk.x = (float)hwconf.xres;
	sKuk.bKUK = 1;
	sKuk.iState = 0;
	sKuk.Speed = 600.0f;
	sKuk.dwTCounter = 0;
}

void _3d_Process_Kuk(float *_3d_Scale_Factor)
{
	float pos[4];
	float f = sKuk.Speed * (ber.TimeLastFrame / 1000.0f);

	switch(sKuk.iState)
	{
	case 0:
		sKuk.x -= f;

		if(sKuk.x < (hwconf.xres - (128 * _3d_Scale_Factor[0])))
		{
			sKuk.x = (hwconf.xres - (128 * _3d_Scale_Factor[0]));
			sKuk.iState = 1;
		}
		break;
	case 1:
		sKuk.dwTCounter += ber.TimeLastFrame;

		if(sKuk.dwTCounter > 1000)
			sKuk.iState = 2;
		break;
	case 2:
		sKuk.x += f;

		if(sKuk.x > hwconf.xres)
		{
			sKuk.bKUK = 0;
			sKuk.iState = 3;
		}
		break;
	}

	pos[0] = sKuk.x;
	pos[1] = sKuk.y;
	pos[2] = sKuk.x + (128 * _3d_Scale_Factor[0]);
	pos[3] = sKuk.y + (128 * _3d_Scale_Factor[1]);

	//kprintf(1, "%f, %f, %f, %f", pos[0], pos[1], pos[2], pos[3]);

	glBindTexture(GL_TEXTURE_2D, sKuk.text.text);

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2d(0,1);
	glVertex2d(pos[0], pos[1]);
	glTexCoord2d(0,0);
	glVertex2d(pos[0], pos[3]);
	glTexCoord2d(1,1);
	glVertex2d(pos[2], pos[1]);
	glTexCoord2d(1,0);
	glVertex2d(pos[2], pos[3]);
	glEnd();
}

//------------------------------------------------------------------------------------------------
// draws all 3d menus in game to the surface
//------------------------------------------------------------------------------------------------
void _3d_Draw_Menus(char *bCursor, int *Cursor_Time_Out, float *_3d_Scale_Factor,
					LEVELINFO *p_Level, int act_item, int *p_Return, int demo)
{
	float pos[4];

	//updatuj_pozici_mysi();

	if((mi.dx || mi.dy) && cameraflag == -1)
	{
		(*bCursor) = 1;
		(*Cursor_Time_Out) = 0;

		mi.dx = 0;
		mi.dy = 0;
	}

	_3d_Begin_Draw();


	if(p_Level->bTopLedge)
	{
		glCallList(iTopLedgeDL);

		_3d_Draw_Top_Ledge(p_Level, _3d_Scale_Factor, *bCursor);

		p_Return[1] = _3d_Check_Menu_Select(_3d_Scale_Factor, *bCursor, p_Level);

		if(p_Return[1])
		{
			(*bCursor) = 1;
			(*Cursor_Time_Out) = 0;
		}
	}

	if(p_Level->bInventory)
	{
		p_Return[0] = _3d_Draw_Invertory(p_Level, act_item, _3d_Scale_Factor, *bCursor);


		if(p_Level->bMenuRunning == 2 && !p_Level->bRestart && !p_Level->bReturnToMenu)
			_3d_Obsluha_Game_Menu(bCursor, Cursor_Time_Out,
								  p_Level, _3d_Scale_Factor, demo);

		_3d_AnimationEvent();
	}
	else
		p_Return[0] = -1;

	_3d_Draw_3DAnimations();

	_3d_Draw_Indikace(p_Level, _3d_Scale_Factor);

	if(!sKuk.bKUK)
	{
		if(!(rand()%100000))
			_3d_Start_Kuk(_3d_Scale_Factor);
	}
	else
		if(sKuk.bKUK)
			_3d_Process_Kuk(_3d_Scale_Factor);

	if(sHint.bHint)
	{
		_3d_Set_Smooth();
		_3d_Draw_Hint(_3d_Scale_Factor);

		sHint.dwTCounter += ber.TimeLastFrame;

		if(sHint.dwTCounter > 5000)
			sHint.bHint = 0;
	}


	if(p_Level->bRestart)
	{
		(*bCursor) = 1;
		(*Cursor_Time_Out) = 0;
		p_Level->iMessageBoxReturn = _3d_Draw_MessageBox(4, _3d_Scale_Factor);
	}

	if(p_Level->bReturnToMenu)
	{
		(*bCursor) = 1;
		(*Cursor_Time_Out) = 0;
		p_Level->iMessageBoxReturn = _3d_Draw_MessageBox(6, _3d_Scale_Factor);
	}

	if(*bCursor)
	{
		if(!p_Level->iCursor)
		{
			pos[0] = (float)mi.x + _3dCur.iaddx;
			pos[1] = (float)mi.y + _3dCur.iaddy;
			pos[2] = (float)mi.x + _3dCur.idx + _3dCur.iaddx;
			pos[3] = (float)mi.y + _3dCur.idy + _3dCur.iaddy;
		}
		else
		{
			pos[0] = (float)mi.x;
			pos[1] = (float)mi.y;
			pos[2] = (float)mi.x + 32;
			pos[3] = (float)mi.y + 32;
		}

		_3d_Set_Smooth();
		_3d_Draw_Box(p_Level->iCursor, pos);
	}

	_3d_End_Draw();
	
	flip();

	return;
}

void _3d_Run_Cibule(void)
{
	_3d_Start_Animation(15, NULL, NULL);
	_3DAnimationStruct.iCibuleID = _3DAnimationStruct.iLastID - 1;
}

int _3d_Animete_Menu_Item(int iAnimation, int IsInID)
{
	if(mi.t1)
	{
		_3d_Stop_Animation(_3DAnimationStruct.iLastMenuItemID, 1);
		mi.t1 = 0;

		return 1;
	}
	else
		if(_3DAnimationStruct.bLastMenuItem)
		{
			if(_3DAnimationStruct.iLastIsIn == IsInID)
				return 0;
			else
			{
				_3d_Stop_Animation(_3DAnimationStruct.iLastMenuItemID, 1);
				_3d_RemoveIDFromDrawList(_3DAnimationStruct.iLastMenuItemID);
				_3d_Start_Animation(iAnimation, NULL, NULL);
				_3DAnimationStruct.bLastMenuItem = 1;
				_3DAnimationStruct.iLastMenuItemID = _3DAnimationStruct.iLastID -1;
				return 0;
			}
		}
		else
		{
			_3d_Start_Animation(iAnimation, NULL, NULL);
			_3DAnimationStruct.bLastMenuItem = 1;
			_3DAnimationStruct.iLastMenuItemID = _3DAnimationStruct.iLastID -1;
			return 0;
		}

	return 0;
}

void _3d_Cancel_Menu(LEVELINFO *p_Level)
{
		if(_3DAnimationStruct.bLastMenuItem)
		{
			_3d_Stop_Animation(_3DAnimationStruct.iLastMenuItemID, 1);
			_3DAnimationStruct.iLastMenuItemID = -1;
			_3DAnimationStruct.bLastMenuItem = 0;
		}

		_3d_CleanAnimationList();
		_3d_CleanDrawList();
		p_Level->bMenuRunning = 0;
		_3d_Release_From_Video(107);
		fn_Release_3d_Textures();
		p_Level->bInventory = p_Level->bLInventory;
		p_Level->bTopLedge = p_Level->bLastTopLedge;
}

void _3d_Cancel_Animations(LEVELINFO *p_Level)
{
		if(_3DAnimationStruct.bLastMenuItem)
		{
			_3d_Stop_Animation(_3DAnimationStruct.iLastMenuItemID, 1);
			_3DAnimationStruct.iLastMenuItemID = -1;
			_3DAnimationStruct.bLastMenuItem = 0;
		}

		_3d_CleanAnimationList();
		_3d_CleanDrawList();
}

int _3d_HitRect(int iAnimation, float *_3d_Scale_Factor)
{
	_3D_ANIMATION * p_an = &_3DAnimationStruct._3DAnimation[iAnimation];

	if(mi.x > p_an->_3dFrame[0].vfLocation[0] && 
	   mi.x < p_an->_3dFrame[0].vfLocation[0] + (_3dd.p_sysramtexture[p_an->_3dFrame[0].iTexture].tx * _3d_Scale_Factor[0] * p_an->_3dFrame[0].fScale) &&
	   mi.y > p_an->_3dFrame[0].vfLocation[1] &&
	   mi.y < p_an->_3dFrame[0].vfLocation[1] + (_3dd.p_sysramtexture[p_an->_3dFrame[0].iTexture].ty * _3d_Scale_Factor[1] * p_an->_3dFrame[0].fScale))
		return 1;
	else
		return 0;
}

void _3d_Obsluha_Game_Menu(char *bCursor, int *Cursor_Time_Out, LEVELINFO *p_Level,
						   float *_3d_Scale_Factor, int demo)
{
	int bIsIn = 0;

	(*bCursor) = 1;
	(*Cursor_Time_Out) = 0;

	spracuj_spravy(0);
	if(key[K_ESC])
	{
		key[K_ESC] = 0;

		_3d_Cancel_Menu(p_Level);
		return;
	}

	//mainmenu
/*	if(mi.x > (206 * _3d_Scale_Factor[0]) && mi.x < (456 * _3d_Scale_Factor[0]) &&
	   mi.y > (186 * _3d_Scale_Factor[1]) && mi.y < (230 * _3d_Scale_Factor[1]))*/
	if(_3d_HitRect(16, _3d_Scale_Factor))
	{
		bIsIn = 1;

		if(_3d_Animete_Menu_Item(16, 1))
		{
			float pos[3] = {0,0,0};
			int rnd = rand()%3;

			ap_Play_Sound(0,1,0, pos, rnd + 23, NULL, &ad);

			/*_3d_Cancel_Menu(p_Level);
			p_Level->Level_Exit = 2;
			p_Level->lLevel_Exitc = 0;*/

			p_Level->bReturnToMenu = 1;
			p_Level->iMessageBoxReturn = -1;
			p_Level->bCancelMenu = 0;
		}

		_3DAnimationStruct.iLastIsIn = 1;
		return;
	}

	//restart level
	if(_3d_HitRect(17, _3d_Scale_Factor))
	{
		bIsIn = 1;
		
		if(_3d_Animete_Menu_Item(17, 2))
		{
			float pos[3] = {0,0,0};
			int rnd = rand()%3;

			ap_Play_Sound(0,1,0, pos, rnd + 23, NULL, &ad);

			//_3d_Cancel_Menu(p_Level);
			/*p_Level->Level_Exit = 3;
			p_Level->lLevel_Exitc = 0;*/
			p_Level->bRestart = 1;
			p_Level->iMessageBoxReturn = -1;
			p_Level->bCancelMenu = 0;
		}

		_3DAnimationStruct.iLastIsIn = 2;
		return;
	}

	//settings
	if(_3d_HitRect(18, _3d_Scale_Factor))
	{
		bIsIn = 1;
		
		if(_3d_Animete_Menu_Item(18, 4))
		{
			int rnd = rand()%3;
			float pos[3] = {0,0,0};

			ap_Play_Sound(0,1,0, pos, rnd + 23, NULL, &ad);

			RunMenuSettings2("Mmsettings3D.txt", p_Level->p_ad, p_Level, &am);
		}

		_3DAnimationStruct.iLastIsIn = 4;

		return;
	}

	//record demo
	if(_3d_HitRect(19, _3d_Scale_Factor) && !demo)
	{
		bIsIn = 1;
		
		if(_3d_Animete_Menu_Item(19, 5))
		{
			int rnd = rand()%3;
			float pos[3] = {0,0,0};

			ap_Play_Sound(0,1,0, pos, rnd + 23, NULL, &ad);

			p_Level->bSaveDemo = 1;
			_3d_Cancel_Menu(p_Level);
		}

		_3DAnimationStruct.iLastIsIn = 5;

		return;
	}

	if(_3d_HitRect(20, _3d_Scale_Factor))
	{
		bIsIn = 1;
		
		if(_3d_Animete_Menu_Item(20, 6))
		{
			float pos[3] = {0,0,0};
			int rnd = rand()%3;

			ap_Play_Sound(0,1,0, pos, rnd + 23, NULL, &ad);

			_3d_Cancel_Menu(p_Level);
		}

		_3DAnimationStruct.iLastIsIn = 6;
		return;
	}

	//loadgame
	if(_3d_HitRect(21, _3d_Scale_Factor) && !demo)
	{
		bIsIn = 1;
		
		if(_3d_Animete_Menu_Item(21, 7))
		{
			WCHAR cText[128];
			float pos[3] = {0,0,0};
			int rnd = rand()%3;

			ap_Play_Sound(0,1,0, pos, rnd + 23, NULL, &ad);

			ZeroMemory(cText, 128);
			RunMenuLoadGameLoad2("Mmload_game_load3d.txt", p_Level->p_ad, p_Level, &am, 1, cText);

			if(wcslen(cText))
			{
				_3d_Cancel_Menu(p_Level);
				p_Level->Level_Exit = 4;
				p_Level->lLevel_Exitc = 0;
				//TODO	<----------------------- cText = jmeno levelu na load
				
				ZeroMemory(p_Level->cLevelName, 64);
				lsi_Save_Exist(cText, p_Level->cLevelName);
				//WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK | WC_DEFAULTCHAR, cText, wcslen(cText), p_Level->cLevelName, 63, NULL, NULL);
			}
		}

		_3DAnimationStruct.iLastIsIn = 7;
		return;
	}

	//savegame
	if(_3d_HitRect(22, _3d_Scale_Factor) && !demo)
	{
		bIsIn = 1;
		
		if(_3d_Animete_Menu_Item(22, 8))
		{
			WCHAR cText[128];
			float pos[3] = {0,0,0};
			int rnd = rand()%3;

			ap_Play_Sound(0,1,0, pos, rnd + 23, NULL, &ad);

			ZeroMemory(cText, 128);
			RunMenuLoadGameLoad2("Mmload_game_loads3d.txt", p_Level->p_ad, p_Level, &am, 0, cText);

			if(wcslen(cText))
				lsi_Save_Level(cText, p_Level);
		}

		_3DAnimationStruct.iLastIsIn = 8;
		return;
	}

	if(!bIsIn && _3DAnimationStruct.bLastMenuItem)
	{
		_3d_Stop_Animation(_3DAnimationStruct.iLastMenuItemID, 1);
		_3d_RemoveIDFromDrawList(_3DAnimationStruct.iLastMenuItemID);
		_3DAnimationStruct.iLastMenuItemID = -1;
		_3DAnimationStruct.bLastMenuItem = 0;
	}
}

void _3d_Start_Settings(float *_3d_Scale_Factor)
{

	_2d_Init(hwnd_hry);
	_2d_APAK_Load_Bitmap("settings.bmp", pBmpArchive);
	co_CreateDC(_2dd.hDC, 1024, 1024, &hdc_info);

}

int _3d_BackPack2Hint(int idx)
{

	switch(idx)
	{
	case 0:
		return 20;
	case 3:
		return 21;
	case 5:
		return 22;
	case 4:
		return 23;
	case 7:
		return 24;
	case 8:
		return 25;
	}

	kprintf(1,"_3d_BackPack2Hint vraci -1");

	return -1;
}

int _3d_Mesh2Hint(LEVELINFO *p_Level, int mesh)
{
	OBJECTDESC *pObj;
	int i;

	for(i=0;i<p_Level->Size_of_Level;i++)
		if(p_Level->Level[i])
			if(p_Level->Level[i]->Index_Of_Game_Mesh == mesh)
			{
				pObj = p_Level->Level[i]->p_Object;

				if(pObj->Class == 2)
					return 1;
				if(pObj->Class == 19)
					return 2;
				if(pObj->Class == 1)
					return 3;
				if(pObj->Class == 4)
					return 4;
				if(pObj->Class == 5 && pObj->SubClass == 1)
					return 5;
				if(pObj->Class == 5 && !pObj->SubClass)
					return 6;
				if(pObj->Class == 5 && pObj->SubClass == 3)
					return 7;
				if(pObj->Class == 6 && !pObj->SubClass)
					return 8;
				if(pObj->Class == 6 && pObj->SubClass)
					return 9;
				if(pObj->Class == 7)
					return 10;
				if(pObj->Class == 9 && !pObj->SubClass)
					return 11;
				if(pObj->Class == 9 && pObj->SubClass)
					return 12;
				if(pObj->Class == 10)
					return 13;
				if(pObj->Class == 11 && !pObj->SubClass)
					return 14;
				if(pObj->Class == 11 && pObj->SubClass == 1)
					return 15;
				if(pObj->Class == 11 && pObj->SubClass == 3)
					return 16;
				if(pObj->Class == 12)
					return 17;
				if(pObj->Class == 15)
					return 18;
				if(pObj->Class == 16)
					return 19;
				if(pObj->Class == 13 && !pObj->SubClass)
					return 20;
				if(pObj->Class == 13 && pObj->SubClass == 3)
					return 21;
				if(pObj->Class == 13 && pObj->SubClass == 5)
					return 22;
				if(pObj->Class == 13 && pObj->SubClass == 4)
					return 23;
				if(pObj->Class == 13 && pObj->SubClass == 7)
					return 24;
				if(pObj->Class == 13 && pObj->SubClass == 8)
					return 25;

				kprintf(1,"Nenalezena Trida: Class == %d, Sub == %d", pObj->Class, pObj->SubClass);

				return -1;
			}

	kprintf(1,"Nenalezen Prvek : mesh == %d", mesh);

	return -1;
}

void _3d_Draw_Hint(float *_3d_Scale_Factor)
{
	float pos[4];


	pos[0] = sHint.x - (6 * _3d_Scale_Factor[0]);
	pos[1] = sHint.y - (6 * _3d_Scale_Factor[1]);
	pos[2] = (sHint.x - (6 * _3d_Scale_Factor[0])) + (((pHintTexture[sHint.iHint].tx + 12) * HINT_SCALE) * _3d_Scale_Factor[0]);
	pos[3] = (sHint.y - (6 * _3d_Scale_Factor[1])) + (((pHintTexture[sHint.iHint].ty + 12) * HINT_SCALE) * _3d_Scale_Factor[1]);

	glBindTexture(GL_TEXTURE_2D, pHintTexture[0].text.text);

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2d(0,1);
	glVertex2d(pos[0], pos[1]);
	glTexCoord2d(0,0);
	glVertex2d(pos[0], pos[3]);
	glTexCoord2d(1,1);
	glVertex2d(pos[2], pos[1]);
	glTexCoord2d(1,0);
	glVertex2d(pos[2], pos[3]);
	glEnd();

	pos[0] = sHint.x;
	pos[1] = sHint.y;
	pos[2] = sHint.x + (pHintTexture[sHint.iHint].x * HINT_SCALE * _3d_Scale_Factor[0]);
	pos[3] = sHint.y + (pHintTexture[sHint.iHint].y * HINT_SCALE * _3d_Scale_Factor[1]);

	glBindTexture(GL_TEXTURE_2D, pHintTexture[sHint.iHint].text.text);

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2d(0,1);
	glVertex2d(pos[0], pos[1]);
	glTexCoord2d(0,0);
	glVertex2d(pos[0], pos[3]);
	glTexCoord2d(1,1);
	glVertex2d(pos[2], pos[1]);
	glTexCoord2d(1,0);
	glVertex2d(pos[2], pos[3]);
	glEnd();

}

void _3d_Display_Hint(LEVELINFO *p_Level, float *_3d_Scale_Factor)
{
	int iHint;
	int bmesh = kom_get_mesh_mys(12);

	if(bmesh == -1)
	{
		kprintf(1, "Hint = kom_get_mesh_mys() == -1");
		return;
	}

	iHint = _3d_Mesh2Hint(p_Level, bmesh);

	if(iHint == -1)
	{
		kprintf(1, "Hint = _3d_Mesh2Hint() == -1");
		return;
	}

	sHint.iHint = iHint;

	sHint.x = (float)mi.x + _3dCur.idx + _3dCur.iaddx;
	sHint.y = (float)mi.y + _3dCur.idy + _3dCur.iaddy;

	if(sHint.y + (pHintTexture[sHint.iHint].ty * HINT_SCALE * _3d_Scale_Factor[1]) > hwconf.yres - 25)
		sHint.y = (float)(hwconf.yres - (pHintTexture[sHint.iHint].ty * HINT_SCALE * _3d_Scale_Factor[1]) - 25);

	if(sHint.x + (pHintTexture[sHint.iHint].tx * HINT_SCALE * _3d_Scale_Factor[0]) > hwconf.xres - 25)
		sHint.x = (float)(hwconf.xres - (pHintTexture[sHint.iHint].tx * HINT_SCALE * _3d_Scale_Factor[0]) - 25);

	sHint.bHint = 1;
	sHint.dwTCounter = 0;
}

void _3d_Display_Hint_Inventory(LEVELINFO *p_Level, ITEMDESC *pItem, int idx, int x, int y, float *_3d_Scale_Factor)
{
	int obj;
	int iHint;

	obj = gl_Inventory_Item_To_Object(p_Level, idx, 0);

	if(obj >= p_Level->Count_Of_Objects)
	{
		kprintf(1, "nepodarilo se prelozit index inventoraze na objekt!");
		return;
	}

	iHint = _3d_BackPack2Hint(p_Level->Object[obj].SubClass);

	if(iHint == -1)
		return;

	
	sHint.iHint = iHint;

	sHint.x = x * _3d_Scale_Factor[0];
	sHint.y = (y - 32) * _3d_Scale_Factor[1];

	if(sHint.x + (pHintTexture[sHint.iHint].tx * HINT_SCALE * _3d_Scale_Factor[0]) > hwconf.xres - 25)
		sHint.x = (float)(hwconf.xres - (pHintTexture[sHint.iHint].tx * HINT_SCALE * _3d_Scale_Factor[0]) - 25);

	sHint.bHint = 1;
	sHint.dwTCounter = 0;
}

void _3d_Nahraj_Kuk(void)
{
	txt_trida(TEXT_MENU);
	kom_set_default_text_config(0,0,1,0,0,1);
	txt_nahraj_texturu_z_bmp(p3DMArchive,"brouk1.bmp", &sKuk.text, 0, 1, NULL);
	kom_ret_default_text_config();
}

void _3d_Release_Kuk(void)
{
	txt_zrus_texturu(&sKuk.text);
}

void _3d_Draw_Indikace(LEVELINFO *p_Level, float *_3d_Scale_Factor)
{
	float pos[4];

	if(p_Level->bSaveDemo)
	{
		pos[0] = 960 * _3d_Scale_Factor[0];
		pos[1] = 704 * _3d_Scale_Factor[1];
		pos[2] = 1024 * _3d_Scale_Factor[0];
		pos[3] = 768 * _3d_Scale_Factor[1];

		glBindTexture(GL_TEXTURE_2D, sIndikace[0].text);

		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(0,1);
		glVertex2d(pos[0], pos[1]);
		glTexCoord2d(0,0);
		glVertex2d(pos[0], pos[3]);
		glTexCoord2d(1,1);
		glVertex2d(pos[2], pos[1]);
		glTexCoord2d(1,0);
		glVertex2d(pos[2], pos[3]);
		glEnd();
	}

	if(p_Level->Item_Lock)
	{
		pos[0] = 896 * _3d_Scale_Factor[0];
		pos[1] = 704 * _3d_Scale_Factor[1];
		pos[2] = 960 * _3d_Scale_Factor[0];
		pos[3] = 768 * _3d_Scale_Factor[1];

		glBindTexture(GL_TEXTURE_2D, sIndikace[2].text);

		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(0,1);
		glVertex2d(pos[0], pos[1]);
		glTexCoord2d(0,0);
		glVertex2d(pos[0], pos[3]);
		glTexCoord2d(1,1);
		glVertex2d(pos[2], pos[1]);
		glTexCoord2d(1,0);
		glVertex2d(pos[2], pos[3]);
		glEnd();
	}

	if(p_Level->Actual_Item == -1)
		return;

	if(p_Level->Level[p_Level->Actual_Item]->p_Back_Pack->Strength > 2)
	{
		pos[0] = 832 * _3d_Scale_Factor[0];
		pos[1] = 704 * _3d_Scale_Factor[1];
		pos[2] = 896 * _3d_Scale_Factor[0];
		pos[3] = 768 * _3d_Scale_Factor[1];

		glBindTexture(GL_TEXTURE_2D, sIndikace[1].text);

		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(0,1);
		glVertex2d(pos[0], pos[1]);
		glTexCoord2d(0,0);
		glVertex2d(pos[0], pos[3]);
		glTexCoord2d(1,1);
		glVertex2d(pos[2], pos[1]);
		glTexCoord2d(1,0);
		glVertex2d(pos[2], pos[3]);
		glEnd();
	}
}

int _3d_Draw_MessageBox(int iMessage, float *_3d_Scale_Factor)
{
	int	ret = 0;
	float pos[4];
	float	x = (1024 - (pMessageTexture[iMessage].tx + 12 )) / 2.0f;
	float	y = (768 - ((pMessageTexture[iMessage].ty * 3) + 12)) / 2.0f;
	float	dx;
	float	dy;
	float	f;

	pos[0] = (x - 6) * _3d_Scale_Factor[0];
	pos[1] = (y - 6) * _3d_Scale_Factor[1];
	pos[2] = (x - 6) * _3d_Scale_Factor[0] + (((pMessageTexture[iMessage].tx + 12) * HINT_SCALE) * _3d_Scale_Factor[0]);
	pos[3] = (y - 6) * _3d_Scale_Factor[1] + ((((pMessageTexture[iMessage].ty * 3) + 12) * HINT_SCALE) * _3d_Scale_Factor[1]);

	dx = (pMessageTexture[iMessage].tx + 12 ) / 2.0f;
	dy = pMessageTexture[iMessage].ty * 2.0f;

	glBindTexture(GL_TEXTURE_2D, pHintTexture[0].text.text);

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2d(0,1);
	glVertex2d(pos[0], pos[1]);
	glTexCoord2d(0,0);
	glVertex2d(pos[0], pos[3]);
	glTexCoord2d(1,1);
	glVertex2d(pos[2], pos[1]);
	glTexCoord2d(1,0);
	glVertex2d(pos[2], pos[3]);
	glEnd();

	pos[0] = x * _3d_Scale_Factor[0];
	pos[1] = y * _3d_Scale_Factor[1];
	pos[2] = (x + (pMessageTexture[iMessage].x * HINT_SCALE)) * _3d_Scale_Factor[0];
	pos[3] = (y + (pMessageTexture[iMessage].y * HINT_SCALE)) * _3d_Scale_Factor[1];

	glBindTexture(GL_TEXTURE_2D, pMessageTexture[iMessage].text.text);

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2d(0,1);
	glVertex2d(pos[0], pos[1]);
	glTexCoord2d(0,0);
	glVertex2d(pos[0], pos[3]);
	glTexCoord2d(1,1);
	glVertex2d(pos[2], pos[1]);
	glTexCoord2d(1,0);
	glVertex2d(pos[2], pos[3]);
	glEnd();

	f = x + dx - pMessageTexture[0].tx - 10;

	pos[0] = f * _3d_Scale_Factor[0];
	pos[1] = (y + dy) * _3d_Scale_Factor[1];
	pos[2] = (f + (pMessageTexture[0].x * HINT_SCALE)) * _3d_Scale_Factor[0];
	pos[3] = (y + dy + (pMessageTexture[0].y * HINT_SCALE)) * _3d_Scale_Factor[1];

	ret = -1;

	if(mi.x >= pos[0] && mi.x <= pos[0] + (pMessageTexture[1].tx * _3d_Scale_Factor[0]) &&
	   mi.y >= pos[1] && mi.y <= pos[1] + (pMessageTexture[1].ty * _3d_Scale_Factor[1]))
	{
		glBindTexture(GL_TEXTURE_2D, pMessageTexture[1].text.text);
		ret = 1;
	}
	else
		glBindTexture(GL_TEXTURE_2D, pMessageTexture[0].text.text);

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2d(0,1);
	glVertex2d(pos[0], pos[1]);
	glTexCoord2d(0,0);
	glVertex2d(pos[0], pos[3]);
	glTexCoord2d(1,1);
	glVertex2d(pos[2], pos[1]);
	glTexCoord2d(1,0);
	glVertex2d(pos[2], pos[3]);
	glEnd();

	f = x + dx + 10;

	pos[0] = f * _3d_Scale_Factor[0];
	pos[1] = (y + dy) * _3d_Scale_Factor[1];
	pos[2] = (f + (pMessageTexture[2].x * HINT_SCALE)) * _3d_Scale_Factor[0];
	pos[3] = (y + dy + (pMessageTexture[2].y * HINT_SCALE)) * _3d_Scale_Factor[1];

	if(mi.x >= pos[0] && mi.x <= pos[0] + (pMessageTexture[1].tx * _3d_Scale_Factor[0]) &&
	   mi.y >= pos[1] && mi.y <= pos[1] + (pMessageTexture[1].ty * _3d_Scale_Factor[1]))
	{
		glBindTexture(GL_TEXTURE_2D, pMessageTexture[3].text.text);
		ret = 0;
	}
	else
		glBindTexture(GL_TEXTURE_2D, pMessageTexture[2].text.text);

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2d(0,1);
	glVertex2d(pos[0], pos[1]);
	glTexCoord2d(0,0);
	glVertex2d(pos[0], pos[3]);
	glTexCoord2d(1,1);
	glVertex2d(pos[2], pos[1]);
	glTexCoord2d(1,0);
	glVertex2d(pos[2], pos[3]);
	glEnd();

	if(key[K_ESC])
	{
		key[K_ESC] = 0;
		mi.t1 = 1;
		ret = 0;
	}

	if(key[K_ENTER])
	{
		key[K_ENTER] = 0;
		mi.t1 = 1;
		ret = 1;
	}

	return ret;
}