/****************
 * Include file of XEXP.C: xexpfun.c
 * PURPOSE:
 *     functions service for xexp.c's user
 *
 * Copyright (c) Shanghai Instituate of Railway Technology, 1992
 *               CRSC 1998
 *
 * 1. let _TypeAlign support DATE_TYPE
 ****************************************************************************/

//#define USE_BTREE_FOR_UNIQUE

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <errno.h>
#include <io.h>
#include <dos.h>
#include <time.h>

#include "mistring.h"
#include "mistools.h"
#include "asqlana.h"
#include "dio.h"
#include "dioext.h"
#include "xexp.h"
#include "btree.h"
#include "strutl.h"
#include "hzth.h"
#include "cfg_fast.h"
#include "ts_dict.h"
#include "des.h"
#include "asqlpath.h"
#include "btree.h"
#include "btreeext.h"

#ifdef __N_C_S
#include "cfg_fast.h"
#include "msgbox.h"
#endif

extern ErrorType ErrorSet;


int char_sort_function( const void *a, const void *b);
int short_sort_function( const void *a, const void *b);
int long_sort_function( const void *a, const void *b);
int float_sort_function( const void *a, const void *b);


/*------------
 !                      _xAtol()
 !-----------------------------------------------------------------------*/
short PD_style _xAtol( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    if( lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN ) {
	*(long *)lpOpnd[0].values = \
				atol( subcopy((char *)*(long *)lpOpnd[0].oval,\
				0, lpOpnd[0].length ) );
    } else {
	if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
		if( lpOpnd[0].oval == NULL ) {
			*(long *)lpOpnd[0].values = \
				atol( (char *)*(long *)lpOpnd[0].values );
			free((char *)*(long *)lpOpnd[0].values);
		} else {
			*(long *)lpOpnd[0].values = \
				atol(SubstrOutBlank(lpOpnd[0].oval,0,lpOpnd[0].length));
		}
	} else {
		if( lpOpnd[0].oval == NULL ) {
			*(long *)lpOpnd[0].values = atol((char *)lpOpnd[0].values);
		} else {
			*(long *)lpOpnd[0].values = atol((char *)lpOpnd[0].oval);
		}
	}
    }

    lpOpnd[0].type = LONG_TYPE;

    *OpndTop -= ParaNum - 1;
    return( 0 );

} /* end of function _xAtol() */



/*------------
 !                      _xMax()
 !-----------------------------------------------------------------------*/
short PD_style _xMax( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    short i;

    if( _TypeAlign( lpOpnd, ParaNum, lpOpnd[0].type ) ) {
	if( _TypeAlign( lpOpnd, ParaNum, lpOpnd[1].type ) ) {
		return( 1 );
	} else
	switch( lpOpnd[1].type ) {
	    case FLOAT_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
		    if( *(double *)lpOpnd[0].values < *(double *)lpOpnd[i].values )
			*(double *)lpOpnd[0].values = *(double *)lpOpnd[i].values;
		lpOpnd[0].type = FLOAT_TYPE;
		break;
	    case LONG_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
		    if( *(long *)lpOpnd[0].values < *(long *)lpOpnd[i].values )
			*(long *)lpOpnd[0].values = *(long *)lpOpnd[i].values;
		lpOpnd[0].type = LONG_TYPE;
		break;
	    case DATE_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
		    if( *(long *)lpOpnd[0].values < *(long *)lpOpnd[i].values )
			*(long *)lpOpnd[0].values = *(long *)lpOpnd[i].values;
		DateAddToLong( "", *(long *)lpOpnd[0].values, lpOpnd[0].values);
		lpOpnd[0].type = DATE_TYPE;
		lpOpnd[0].length = 8;
		break;
	}
    } else {
	switch( lpOpnd[0].type ) {
	    case FLOAT_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
		    if( *(double *)lpOpnd[0].values < *(double *)lpOpnd[i].values )
			*(double *)lpOpnd[0].values = *(double *)lpOpnd[i].values;
		break;
	    case LONG_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
		    if( *(long *)lpOpnd[0].values < *(long *)lpOpnd[i].values )
			*(long *)lpOpnd[0].values = *(long *)lpOpnd[i].values;
		break;
	    case DATE_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
		    if( *(long *)lpOpnd[0].values < *(long *)lpOpnd[i].values )
			*(long *)lpOpnd[0].values = *(long *)lpOpnd[i].values;
		DateAddToLong( "", *(long *)lpOpnd[0].values, lpOpnd[0].values);
		lpOpnd[0].type = DATE_TYPE;
		lpOpnd[0].length = 8;
		break;
	}
    }
    *OpndTop -= ParaNum - 1;

    return  0;

} /* end of function _xMax() */


/*------------
 !                      _xMin()
 !-----------------------------------------------------------------------*/
short PD_style _xMin( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    short i;

    if( _TypeAlign( lpOpnd, ParaNum, lpOpnd[0].type ) ) {
	if( _TypeAlign( lpOpnd, ParaNum, lpOpnd[1].type ) ) {
		return( 1 );
	} else
	switch( lpOpnd[1].type ) {
	    case FLOAT_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
		    if( *(double *)lpOpnd[0].values > *(double *)lpOpnd[i].values )
			*(double *)lpOpnd[0].values = *(double *)lpOpnd[i].values;
		lpOpnd[0].type = FLOAT_TYPE;
		//lpOpnd[0].length = 8;
		break;
	    case LONG_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
		    if( *(long *)lpOpnd[0].values > *(long *)lpOpnd[i].values )
			*(long *)lpOpnd[0].values = *(long *)lpOpnd[i].values;
		lpOpnd[0].type = LONG_TYPE;
		//lpOpnd[0].length = 4;
		break;
	    case DATE_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
		    if( *(long *)lpOpnd[0].values > *(long *)lpOpnd[i].values )
			*(long *)lpOpnd[0].values = *(long *)lpOpnd[i].values;
		DateAddToLong( "", *(long *)lpOpnd[0].values, lpOpnd[0].values);
		lpOpnd[0].type = DATE_TYPE;
		lpOpnd[0].length = 8;
		break;
	}
    } else {
	switch( lpOpnd[0].type ) {
	    case FLOAT_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
		    if( *(double *)lpOpnd[0].values > *(double *)lpOpnd[i].values )
			*(double *)lpOpnd[0].values = *(double *)lpOpnd[i].values;
		break;
	    case LONG_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
		    if( *(long *)lpOpnd[0].values > *(long *)lpOpnd[i].values )
			*(long *)lpOpnd[0].values = *(long *)lpOpnd[i].values;
		break;
	    case DATE_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
		    if( *(long *)lpOpnd[0].values > *(long *)lpOpnd[i].values )
			*(long *)lpOpnd[0].values = *(long *)lpOpnd[i].values;
		DateAddToLong( "", *(long *)lpOpnd[0].values, lpOpnd[0].values);
		lpOpnd[0].type = DATE_TYPE;
		lpOpnd[0].length = 8;
		break;
	}
    }
    *OpndTop -= ParaNum - 1;

    return  0;

} /* end of function _xMin() */


/*------------
 !                      _xSum()
 !-----------------------------------------------------------------------*/
short PD_style _xSum( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{

    short i;

    if( _TypeAlign( lpOpnd, ParaNum, lpOpnd[0].type ) ) {
	if( _TypeAlign( lpOpnd, ParaNum, lpOpnd[1].type ) ) {
		return( 1 );
	} else
	switch( lpOpnd[1].type ) {
	    case FLOAT_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
			*(double *)lpOpnd[0].values += *(double *)lpOpnd[1].values;
		break;
	    case LONG_TYPE:
	    case DATE_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
			*(long *)lpOpnd[0].values += *(long *)lpOpnd[1].values;
	}
    } else {
	switch( lpOpnd[0].type ) {
	    case FLOAT_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
			*(double *)lpOpnd[0].values += *(double *)lpOpnd[1].values;
		break;
	    case LONG_TYPE:
	    case DATE_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
			*(long *)lpOpnd[0].values += *(long *)lpOpnd[1].values;
	}
    }
    *OpndTop -= ParaNum - 1;

    return( 0 );
} /* end of function _xSum() */


/*------------
 !                      _xAverage()
 !-----------------------------------------------------------------------*/
short PD_style _xAverage( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{

    short i;

    if( _TypeAlign( lpOpnd, ParaNum, lpOpnd[0].type ) ) {
	if( _TypeAlign( lpOpnd, ParaNum, lpOpnd[1].type ) ) {
		return( 1 );
	} else
	switch( lpOpnd[1].type ) {
	    case FLOAT_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
			*(double *)lpOpnd[0].values += *(double *)lpOpnd[1].values;
		*(double *)lpOpnd[0].values /= ParaNum;
		break;
	    case LONG_TYPE:
	    case DATE_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
			*(long *)lpOpnd[0].values += *(long *)lpOpnd[1].values;
		*(long *)lpOpnd[0].values /= ParaNum;
	}
    } else {
	switch( lpOpnd[0].type ) {
	    case FLOAT_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
			*(double *)lpOpnd[0].values += *(double *)lpOpnd[1].values;
		*(double *)lpOpnd[0].values /= ParaNum;
		break;
	    case LONG_TYPE:
	    case DATE_TYPE:
		for( i = 1;  i < ParaNum;   i++ )
			*(long *)lpOpnd[0].values += *(long *)lpOpnd[1].values;
		*(long *)lpOpnd[0].values /= ParaNum;
	}
    }
    *OpndTop -= ParaNum - 1;
    return( 0 );

} /* end of function _xAverage */


/*------------
 !                      _xSubstr()
 !-----------------------------------------------------------------------*/
short PD_style _xSubstr( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{

    char *p;

    if( (short)*(long *)lpOpnd[1].values < 1 || \
				(short)*(long *)lpOpnd[2].values < 1 ) {
	return  ErrorSet.xERROR = iSubstrStartErr;	// substr over
    }

    // if the source is stored undirect
    if( (lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN ) || \
					lpOpnd[0].length >= MAX_OPND_LENGTH )
    {

	// if need alloc memory
	if( (short)*(long *)lpOpnd[2].values >= MAX_OPND_LENGTH ) {

		// alloc memory
		if( lpOpnd[0].oval==NULL && \
		    (lpOpnd[0].type < FIELD_IDEN || lpOpnd[0].type > SFIELD_IDEN) )
			p = (char *)realloc( (char *)*(long *)lpOpnd[0].values, \
				      (short)*(long *)lpOpnd[2].values + 1 );
		else    p = (char *)malloc( (short)*(long *)lpOpnd[2].values + 1 );

		if( p == NULL ) {
			return  ErrorSet.xERROR = iNoMem;
		}

		// copy string
		if( lpOpnd[0].oval==NULL )
		{ //variable string
		   strcpy(p, SubstrOutBlank((char *)(*(long *)lpOpnd[0].values), \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values ) );
		} else {
		   if( lpOpnd[0].type == STRING_TYPE )
		   { //const string
			strcpy(p, SubstrOutBlank(lpOpnd[0].oval, \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values ) );
		   } else
		   { //field type
			char buf[256];

			//get the field's data
			strZcpy(buf, (char *)*(long *)lpOpnd[0].oval,\
							lpOpnd[0].length+1 );
			strcpy(p, SubstrOutBlank(buf, \
				  (short)(*(long *)lpOpnd[1].values-1), \
				  (short)*(long *)lpOpnd[2].values ) );
		   }
		}

		// set value
		*(long *)lpOpnd[0].values = (long)p;

	} else
	{ //the result needn't alloc memory
		if( lpOpnd[0].oval==NULL )
		{ //variable string
		    p = (char *)*(long *)lpOpnd[0].values;
		    strcpy( (char *)lpOpnd[0].values, SubstrOutBlank( p, \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values ) );
		    free(p);
		} else {
		    if(	lpOpnd[0].type == STRING_TYPE )
		    { //const string
			strcpy( (char *)lpOpnd[0].values, \
					SubstrOutBlank(lpOpnd[0].oval, \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values) );
		    } else
		    { //field type
			char buf[256];

			strZcpy(buf, (char *)*(long *)lpOpnd[0].oval,\
							lpOpnd[0].length+1 );
			strcpy( (char *)lpOpnd[0].values, \
					SubstrOutBlank( buf, \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values ) );
		    }
		}
	}
    } else if( lpOpnd[0].type != STRING_TYPE ) {
	return  1;
    } else
    { //the lpOpnd[0] memory should not be free

	// whether need to alloc memory
	if( (short)*(long *)lpOpnd[2].values >= MAX_OPND_LENGTH ) {
		p = (char *)malloc( (short)*(long *)lpOpnd[2].values + 1 );
		if( lpOpnd[0].oval==NULL ) {
			strcpy( p, SubstrOutBlank( (char *)*(long *)lpOpnd[0].values, \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values ) );
		} else {
			strcpy( p, SubstrOutBlank( (char *)*(long *)lpOpnd[0].oval, \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values ) );
		}

		(*(long *)lpOpnd[0].values) = (long)p;
	} else {
		if( lpOpnd[0].oval==NULL ) {
			strcpy((char *)lpOpnd[0].values, \
					SubstrOutBlank( (char *)lpOpnd[0].values, \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values ) );
		} else {
			strcpy((char *)lpOpnd[0].values, \
					SubstrOutBlank( (char *)lpOpnd[0].oval, \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values ) );
		}
	}
    }

    lpOpnd[0].oval = NULL;
    lpOpnd[0].type = STRING_TYPE;
    lpOpnd[0].length = (short)*(long *)lpOpnd[2].values;

    *OpndTop -= ParaNum - 1;
    return( 0 );

}


/*------------
 !                      _xSubstrB()
 !-----------------------------------------------------------------------*/
short PD_style _xSubstrB( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{

    char *p;

    // not enough parameter
    if( (short)*(long *)lpOpnd[1].values < 1 ) {
	return  ErrorSet.xERROR = iSubstrStartErr;		/* substr over */
    }

    // if the source is stored undirect
    if( (lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN ) || \
					lpOpnd[0].length >= MAX_OPND_LENGTH )
    {

	// if need alloc memory
	if( (short)*(long *)lpOpnd[2].values >= MAX_OPND_LENGTH ) {

		// alloc memory
		if( lpOpnd[0].oval==NULL && \
		    (lpOpnd[0].type < FIELD_IDEN || lpOpnd[0].type > SFIELD_IDEN) )
			p = (char *)realloc( (char *)*(long *)lpOpnd[0].values, \
				      (short)*(long *)lpOpnd[2].values + 1 );
		else    p = (char *)malloc( (short)*(long *)lpOpnd[2].values + 1 );

		if( p == NULL ) {
			return  ErrorSet.xERROR = iNoMem;
		}

		// copy string
		if( lpOpnd[0].oval==NULL )
		{ //variable string
		   strcpy(p, substr((char *)(*(long *)lpOpnd[0].values), \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values ) );
		} else {
		   if( lpOpnd[0].type == STRING_TYPE )
		   { //const string
			strcpy(p, substr(lpOpnd[0].oval, \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values ) );
		   } else
		   { //field type
			char buf[256];

			//get the field's data
			strZcpy(buf, (char *)*(long *)lpOpnd[0].oval,\
							lpOpnd[0].length+1 );
			strcpy(p, substr(buf, \
				  (short)(*(long *)lpOpnd[1].values-1), \
				  (short)*(long *)lpOpnd[2].values ) );
		   }
		}

		// set value
		*(long *)lpOpnd[0].values = (long)p;

	} else
	{ //the result needn't alloc memory
		if( lpOpnd[0].oval==NULL )
		{ //variable string
		    p = (char *)*(long *)lpOpnd[0].values;
		    strcpy( (char *)lpOpnd[0].values, substr( p, \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values ) );
		    free(p);
		} else {
		    if(	lpOpnd[0].type == STRING_TYPE )
		    { //const string
			strcpy( (char *)lpOpnd[0].values, \
					substr(lpOpnd[0].oval, \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values) );
		    } else
		    { //field type
			char buf[256];

			strZcpy(buf, (char *)*(long *)lpOpnd[0].oval,\
							lpOpnd[0].length+1 );
			strcpy( (char *)lpOpnd[0].values, \
					substr( buf, \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values ) );
		    }
		}
	}
    } else if( lpOpnd[0].type != STRING_TYPE ) {
	return  1;
    } else
    { //the lpOpnd[0] memory should not be free

	// whether need to alloc memory
	if( (short)*(long *)lpOpnd[2].values >= MAX_OPND_LENGTH ) {
		p = (char *)malloc( (short)*(long *)lpOpnd[2].values + 1 );
		if( lpOpnd[0].oval==NULL ) {
			strcpy( p, substr( (char *)*(long *)lpOpnd[0].values, \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values ) );
		} else {
			strcpy( p, substr( (char *)*(long *)lpOpnd[0].oval, \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values ) );
		}

		(*(long *)lpOpnd[0].values) = (long)p;
	} else {
		if( lpOpnd[0].oval==NULL ) {
			strcpy((char *)lpOpnd[0].values, \
					substr( (char *)lpOpnd[0].values, \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values ) );
		} else {
			strcpy((char *)lpOpnd[0].values, \
					substr( (char *)lpOpnd[0].oval, \
					(short)(*(long *)lpOpnd[1].values-1), \
					(short)*(long *)lpOpnd[2].values ) );
		}
	}
    }

    lpOpnd[0].oval = NULL;
    lpOpnd[0].type = STRING_TYPE;
    lpOpnd[0].length = (short)*(long *)lpOpnd[2].values;

    *OpndTop -= ParaNum - 1;
    return( 0 );

}


/*------------
 !                      _xNull()
 !-----------------------------------------------------------------------*/
short PD_style _xNull( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
//    short  i;
    char  *sz;

    //suport 1 parameter only
    /*for( i = 0;  i < ParaNum;  i++ ) */{

	//sz = xGetOpndString(&lpOpnd[i]);
	sz = xGetOpndString(&lpOpnd[0]);
	if( sz == NULL ) {
	    *(short *)lpOpnd[0].values = 1;
	    //break;
	}

	if( lrtrim(sz)[0] == '\0' ) {
	    *(short *)lpOpnd[0].values = 1;
	} else {
	    *(short *)lpOpnd[0].values = 0;
	    //break;
	}
	/* 1998.10.4
	if( lpOpnd[i].type > FIELD_IDEN && lpOpnd[i].type <= SFIELD_IDEN ) {
	    if( lrtrim( substr( (char *)*(long *)lpOpnd[i].oval, 0, \
			    lpOpnd[i].length ) )[0] == '\0' ) {
		 *(short *)lpOpnd[0].values = 1;
	    } else {
		 *(short *)lpOpnd[0].values = 0;
		 break;
	    }
	} else {
	    if( lpOpnd[i].length > 0 ) {
		*(short *)lpOpnd[0].values = 0;
	    } else {
		*(short *)lpOpnd[0].values = 1;
		break;
	    }
	}
	*/
    } /* end of for (i) */

    lpOpnd[0].length = sizeof(short);
    lpOpnd[0].type = LOGIC_TYPE;

    *OpndTop -= ParaNum - 1;

    return  0;

} /* end of function _xNull() */


/*------------
 !                      _xDate()
 !-----------------------------------------------------------------------*/
short PD_style _xDate( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    short y, m, d, i;
    int   isDate;
    char  temp[10];
    char *sz;
    short Table[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    sz = xGetOpndString(&lpOpnd[0]);
    strcpy(temp, substr(sz, 0, 8));

    if( temp[0] == '\0' ) {
	ErrorSet.xERROR = iInVaildOpndType;
	return  1;
    };

    lpOpnd[0].type = DATE_TYPE;
    lpOpnd[0].length = 8;

    y = atoi( substr(temp, 0, 4) );

    if( y > 0 ) {
	for(i = 0;   i < 4 && isdigit(temp[i]);  i++);
	if( i < 4 ) {
		y = -1;
	}
    }
    if( y <= 0 || temp[4] == ' ' ) {
	memset(lpOpnd[0].values, 0, 9);
	*OpndTop -= ParaNum - 1;
	return  0;
    }

    m = atoi( substr(temp, 4, 2) );
    if( m < 0 || m > 12 ) {
	y = -1;
    } else {
	for(i = 4;   i < 6 && isdigit(temp[i]);  i++);
	if( i < 6 ) {
	    y = -1;
	}
    }
    if( y <= 0 /*|| temp[6] == ' '*/ ) {
	memset(lpOpnd[0].values, 0, 9);
	*OpndTop -= ParaNum - 1;
	return( 0 );
    }

    d = atoi( substr(temp, 6, 2) );
    if( m != 2 ) {
	if( d >= 0 && d <= Table [ m-1 ] ) {
		isDate = 1;
	} else {
		if( temp[6] == ' ' && temp[7] == ' ' )
			isDate = 1;
		else
			isDate = 0;
	}
    } else {
	if( y % 4 == 0 && y % 100 != 0 || y % 400 == 0 ) {
		if( d <= 29 )		isDate = 1;
		else	     		isDate = 0;
	} else {
		if( d <= 28 )		isDate = 1;
		else	     		isDate = 0;
	}
    }
    for(i = 6;   i < 8 && (isdigit(temp[i]) || temp[i]==' ');  i++);
    if( i < 8 )	isDate = 0;

    if( isDate )
	strcpy(lpOpnd[0].values, temp);
    else
	memset(lpOpnd[0].values, 0, 9);

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xDate() */


/*------------
 !                      _xPostcode()
 !-----------------------------------------------------------------------*/
short PD_style _xPostcode( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
     char *temp;
     short i;

     if( lpOpnd[0].type < FIELD_IDEN || lpOpnd[0].type > SFIELD_IDEN || \
						lpOpnd[0].length !=6 ) {
		return( 0 );
     }
     temp = (char *)*(long *)lpOpnd[0].oval;

     for( i = 0;   i < 6  && isdigit( temp[i] );   i++ );

     if( i >= 6 ) {
		*(short *)lpOpnd[0].values = 1;
     } else {
		*(short *)lpOpnd[0].values = 0;
     }

    lpOpnd[0].length = sizeof(short);
    lpOpnd[0].type = LOGIC_TYPE;
    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xPostcode() */


/*------------
 !                      _xMoudleString()
 ! mdstr()
 !-----------------------------------------------------------------------*/
short PD_style _xMoudleString( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
     unsigned char *temp, *module;
     short i, nonstop, f0 = 0, f1 = 0;

     if( lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN ) {
	temp = (char *)*(long *)lpOpnd[0].oval;
     } else {
	if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
		if( lpOpnd[0].oval == NULL ) {
			f0 = 1;
			temp = (char *)*(long *)lpOpnd[0].values;
		} else {
			temp = (char *)lpOpnd[0].oval;
		}
	} else {
		if( lpOpnd[0].oval == NULL ) {
			temp = (char *)lpOpnd[0].values;
		} else {
			temp = (char *)lpOpnd[0].oval;
		}
	}
     }
     if( lpOpnd[1].type > FIELD_IDEN && lpOpnd[1].type <= SFIELD_IDEN ) {
	module = (char *)*(long *)lpOpnd[1].oval;
     } else {
	if( lpOpnd[1].length >= MAX_OPND_LENGTH ) {
		if( lpOpnd[1].oval == NULL ) {
			f1 = 1;
			module = (char *)*(long *)lpOpnd[1].values;
		} else {
			module = (char *)lpOpnd[1].oval;
		}
	} else {
		if( lpOpnd[1].oval == NULL ) {
			module = lpOpnd[1].values;
		} else {
			module = (char *)lpOpnd[1].oval;
		}
	}
     }

     nonstop = 1;
     for( i = 0;   i < lpOpnd[0].length && nonstop == 1;   i++ ) {
	switch( module[i] ) {
		case '\0':		// stop
			if( temp[i] )
			    nonstop = 0;
			else
			    i = lpOpnd[0].length + 1;	//break from for
			break;
		case 'N':		// number only
			if( !isdigit( temp[i] ) )  {
				nonstop = 0;
			}
			break;
		case '#':		// number or ' ' or '+' or '-'
			if( !isdigit( temp[i] ) && temp[i] != ' ' && \
			    temp[i] != '+' && temp[i] != '-' )  {
				nonstop = 0;
			}
			break;
		case 'A':		// char only
			if( temp[i] <= ' ' ) { // treat it as char for chinese
				nonstop = 0;
			}
			break;
		case 'E':		// char only
			if( !isalnum(temp[i]) ) {
				nonstop = 0;
			}
			break;
		case 'X':     		// every ascii
			break;
		default:
			if( temp[i] != module[i] && temp[i] ) {
				nonstop = 0;
			}
	} // end of switch
     } // end of for

     if( f0 != 0 )	free( temp );
     if( f1 != 0 )	free( module );

     *(short *)lpOpnd[0].values = nonstop;

     lpOpnd[0].length = sizeof(short);
     lpOpnd[0].type = LOGIC_TYPE;
     *OpndTop -= ParaNum - 1;
     return  0;

} /* end of function _xMoudleString() */


/*------------
 !                      _xUniqueKeyword()
 !
 ! uniquekey(field, key expr, val)
 !-----------------------------------------------------------------------*/
short PD_style _xUniqueKeyword( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
#ifdef ASQLCLT

    *(short *)lpOpnd[0].values = 1;

    lpOpnd[0].type = LOGIC_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

#else

     short i;
     long  l;
#ifdef USE_BTREE_FOR_UNIQUE
     bHEAD *bh;
     char *s;

     if( lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN ) {
	s = (char *)*(long *)lpOpnd[0].oval;
     } else {
	if( (s = (char *)lpOpnd[0].oval) == NULL ) {
		s = lpOpnd[0].values;
	}
     }

     if( (bh = (bHEAD *)*(long *)(SysVar[SysVar_STRINGTMP].values)) == \
								NULL ) {
	*(short *)lpOpnd[0].values = 2;
     } else {
	i = 0;
	l = IndexSeek(bh, s);
	if( l == LONG_MIN ) {
		i = 1;
	} else {
		if( l == *(long *)(SysVar[SysVar_RECNO].values) ) {
			if( IndexEqSkip(bh, 1L) == LONG_MIN ) {
				i = 1;
			}
		}
	} // end of else
	*(short *)lpOpnd[0].values = i;
     }

     lpOpnd[0].type = LOGIC_TYPE;

     *OpndTop -= ParaNum - 1;
     return  0;

#else

    int   k;
    char  *sz;
    bHEAD *bh;
    char  buf[256];
    dFIELDWHENACTION *p;

    if( xIsOpndField(&lpOpnd[0]) )
    { //get field name and its sync index btree
	p = (dFIELDWHENACTION *)lpOpnd[0].oval;
	if( p->pSourceDfile == NULL ) 	return  1;

	if( ParaNum < 2 )
	{ //field name
	    strZcpy(buf, getFieldInfo(p->pSourceDfile, p->wSourceid)->field, 256);
	} else {
	    sz = xGetOpndString(&lpOpnd[1]);
	    strZcpy(buf, sz, 256);
	}

	bh = NULL;
	for(k = 0;   k < p->pSourceDfile->syncBhNum;   k++)
	{
	    if( keyFieldPartCmp((bHEAD *)(p->pSourceDfile->bhs[k]), buf) == 0 ) {
		bh = IndexAwake((char *)(p->pSourceDfile), ((bHEAD *)(p->pSourceDfile->bhs[k]))->ndxName,
									BTREE_FOR_ITSELF);
		break;
	    }
	}

	if( bh == NULL ) {
	    *(short *)lpOpnd[0].values = 1;
	    lpOpnd[0].type = LOGIC_TYPE;

	    *OpndTop -= ParaNum - 1;
	    ErrorSet.xERROR = iNoIndexToUseIn_uniquekey;
	    return  0;
	}
    } else {
	ErrorSet.xERROR = iFirstParaErrIn_uniquekey;
	*(short *)lpOpnd[0].values = 1;
	lpOpnd[0].type = LOGIC_TYPE;

	*OpndTop -= ParaNum - 1;
	ErrorSet.xERROR = iNoIndexToUseIn_uniquekey;
	return  0;
    }

    if( ParaNum >= 3 )
    { //field name
	sz = xGetOpndString(&lpOpnd[2]);
	strZcpy(buf, sz, 256);
    }
    // else use the old info in buf

    //l = IndexKeyCount(bh, buf);
    i = 0;
    l = IndexSeek(bh, buf);
    if( l == LONG_MIN ) {
	i = 1;
    } else {

	//1999.11.20
	//if( l == p->pSourceDfile->rec_no ) {
	//I want to write to the position is rec_p
	//not rec_no
	//
	if( l == p->pSourceDfile->rec_p ) {
		if( IndexStrEqSkip(bh, buf, 1) == LONG_MIN ) {
			i = 1;
		}
	}
    } // end of else

    *(short *)lpOpnd[0].values = i;
    lpOpnd[0].type = LOGIC_TYPE;

    IndexSleep(bh);

    *OpndTop -= ParaNum - 1;
    return  0;

#endif

#endif  //ASQLCLT

} /* end of function _xUniqueKeyword() */


/*------------
 !                      _xLength()
 !-----------------------------------------------------------------------*/
short PD_style _xLength( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
     if( lpOpnd[0].type < FIELD_IDEN || lpOpnd[0].type > SFIELD_IDEN) {
	if( lpOpnd[0].length >= MAX_OPND_LENGTH && lpOpnd[0].oval == NULL ) {
		*(short *)lpOpnd[0].values = strlen( (char *)*(long *)lpOpnd[0].values );
		free( (char *)*(long *)lpOpnd[0].values );
	} else {
		//*(short *)lpOpnd[0].values = lpOpnd[0].length;
		*(short *)lpOpnd[0].values = strlen( lpOpnd[0].values );
	}
     } else {
	*(short *)lpOpnd[0].values = \
		strlen( rtrim(subcopy( (char *)*(long *)lpOpnd[0].oval, \
				       0, lpOpnd[0].length)) );
     }
     lpOpnd[0].type = INT_TYPE;
     *OpndTop -= ParaNum - 1;
     return( 0 );

} /* end of function _xLength() */


/*------------
 !                      _xAge()
 ! protocol: age(birthday)
 !-----------------------------------------------------------------------*/
short PD_style _xAge( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
#ifdef DOS_COMPATABLE
    struct dosdate_t d;
#endif
    char 	buf[10];
    char 	*s;
    char 	buf2[10];

#ifdef DOS_COMPATABLE
    _dos_getdate(&d);
    sprintf(buf, "%04d%02d%02d", d.year, d.month, d.day );
#else
    struct tm t;
    _getsystime(&t);
    sprintf(buf, "%04d%02d%02d", t.tm_year+1900, t.tm_mon, t.tm_mday );
#endif
    
    if( lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN ) {
	s = (char *)*(long *)lpOpnd[0].oval;
    } else {
	// date will store in values
	if( lpOpnd[0].oval == NULL ) {
		s = (char *)lpOpnd[0].values;
	} else {
		s = (char *)lpOpnd[0].oval;
	}
    }
    strncpy(buf2, s, 8);
    buf2[8] = '\0';
    *(long *)lpOpnd[0].values = ((long)DateMinusToLong(buf, buf2))/365;

    lpOpnd[0].type = LONG_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xAge() */


/*------------
 !                      _xSysDate()
 !-----------------------------------------------------------------------*/
short PD_style _xSysDate( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
#ifdef DOS_COMPATABLE
    struct dosdate_t d;

    _dos_getdate(&d);
    if( ParaNum < 1 ) {
	sprintf(lpOpnd[0].values, "%04d%02d%02d", d.year, d.month, d.day );
	lpOpnd[0].type = DATE_TYPE;
	lpOpnd[0].length = 8;
    } else {
	if( lpOpnd[0].type != CHR_TYPE ) {

		char buf[32];

		sprintf(buf, "%04d%02d%02d", d.year, d.month, d.day);
		strZcpy(lpOpnd[0].values, &buf[2], 7);
		lpOpnd[0].type = STRING_TYPE;
		lpOpnd[0].length = 6;

	} else {
		switch( *(char *)(lpOpnd[0].values) ) {
		    case 'm':
		    case 'M':
			lpOpnd[0].type = INT_TYPE;
			*(short *)lpOpnd[0].values = d.month;
			break;
		    case 'd':
		    case 'D':
			lpOpnd[0].type = INT_TYPE;
			*(short *)lpOpnd[0].values = d.day;
			break;
		    /*case 'y':
		    case 'Y':*/
		    default:
			lpOpnd[0].type = INT_TYPE;
			*(short *)lpOpnd[0].values = d.year;
		}
	}
    }
#else
    SYSTEMTIME d;

    GetLocalTime(&d);
    if( ParaNum < 1 ) {
	sprintf(lpOpnd[0].values, "%04d%02d%02d", d.wYear, d.wMonth, d.wDay );
	lpOpnd[0].type = DATE_TYPE;
	lpOpnd[0].length = 8;
    } else {
	if( lpOpnd[0].type != CHR_TYPE ) {

		char buf[32];

		sprintf(buf, "%04d%02d%02d", d.wYear, d.wMonth, d.wDay);
		//strZcpy(lpOpnd[0].values, &buf[2], 7);
		strZcpy(lpOpnd[0].values, buf, 9);
		lpOpnd[0].type = STRING_TYPE;
		lpOpnd[0].length = 8;

	} else {
		switch( *(char *)(lpOpnd[0].values) ) {
		    case 'm':
		    case 'M':
			lpOpnd[0].type = INT_TYPE;
			*(short *)lpOpnd[0].values = d.wMonth;
			break;
		    case 'd':
		    case 'D':
			lpOpnd[0].type = INT_TYPE;
			*(short *)lpOpnd[0].values = d.wDay;
			break;
		    /*case 'y':
		    case 'Y':*/
		    default:
			lpOpnd[0].type = INT_TYPE;
			*(short *)lpOpnd[0].values = d.wYear;
		}
	}
    }
#endif
    lpOpnd[0].oval = NULL;
    *OpndTop -= ParaNum - 1;

    return( 0 );

} /* end of function _xSysDate() */


/*------------
 !                      _xSysTime()
 !-----------------------------------------------------------------------*/
short PD_style _xSysTime( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{

#ifdef DOS_COMPATABLE
    struct dostime_t t;
		
    _dos_gettime(&t);

    sprintf(lpOpnd[0].values, "%02d%02d%02d%02d", \
				    t.hour, t.minute, t.second, t.hsecond);
#else
    
    SYSTEMTIME t;

    GetLocalTime( &t );

    sprintf(lpOpnd[0].values, "%02d%02d%02d%02d", \
				    t.wHour, t.wMinute, t.wSecond,t.wMilliseconds);
#endif
    lpOpnd[0].type = STRING_TYPE;
    lpOpnd[0].length = 8;
    lpOpnd[0].oval = NULL;
    *OpndTop -= ParaNum - 1;
    return( 0 );

} /* end of function _xSysTime() */


/*------------
 !                      _xIsDbfDelete()
 !-----------------------------------------------------------------------*/
short PD_style _xIsDbfDelete( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    dFIELDWHENACTION *p;

    p = (dFIELDWHENACTION *)lpOpnd[0].oval;
    if( p->pSourceDfile == NULL ) 	return  1;

    if( GetDelChar( p->pSourceDfile ) == ' ' ) {
		*(short *)lpOpnd[0].values = 0;
    } else {
		*(short *)lpOpnd[0].values = 1;
    }

    lpOpnd[0].type = INT_TYPE;
    lpOpnd[0].length = sizeof(short);

    *OpndTop -= ParaNum - 1 ;    /* maintain the opnd stack */

    return( 0 );

} /* end of function _xIsDbfDelete() */



/*------------
 !                      _xDbfRecno()
 !-----------------------------------------------------------------------*/
short PD_style _xDbfRecno( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    dFIELDWHENACTION *p;
    dFILE *tdf;
    int    i;
extern WSToMT FromToStru fFrTo;

    if( xIsOpndField(&lpOpnd[0]) == 0 )
    {
	i = xGetOpndLong(&lpOpnd[0]) - 1;
	if( i < 0 || i >= fFrTo.cSouDbfNum )
		return  1;

	tdf = fFrTo.cSouFName[i];
    } else {
	p = (dFIELDWHENACTION *)lpOpnd[0].oval;
	if( (tdf = p->pSourceDfile) == NULL ) 	return  1;
    }

    *(long *)lpOpnd[0].values = tdf->rec_p;
    lpOpnd[0].type = LONG_TYPE;
    lpOpnd[0].length = sizeof(long);

    *OpndTop -= ParaNum - 1 ;    /* maintain the opnd stack */

    return( 0 );

} /* end of function _xDbfRecno() */



/*------------
 !                      _xDbfRecnum()
 !-----------------------------------------------------------------------*/
short PD_style _xDbfRecnum( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    dFIELDWHENACTION *p;
    dFILE *tdf;
    int    i;
extern WSToMT FromToStru fFrTo;

    if( xIsOpndField(&lpOpnd[0]) == 0 )
    {
	i = xGetOpndLong(&lpOpnd[0]) - 1;
	if( i < 0 || i >= fFrTo.cSouDbfNum )
		return  1;

	tdf = fFrTo.cSouFName[i];
    } else {
	p = (dFIELDWHENACTION *)lpOpnd[0].oval;
	if( (tdf = p->pSourceDfile) == NULL ) 	return  1;
    }

    *(long *)lpOpnd[0].values = getRecNum( tdf );
    lpOpnd[0].type = LONG_TYPE;
    lpOpnd[0].length = sizeof(long);

    *OpndTop -= ParaNum - 1 ;    //maintain the opnd stack

    return( 0 );

} /* end of function _xDbfRecnum() */



/*------------
 !                      _xVal()
 !-----------------------------------------------------------------------*/
short PD_style _xVal( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{

    char buf[256];

    if( lpOpnd[0].length >= MAX_OPND_LENGTH || \
	   (lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN) ) {
		if( lpOpnd[0].oval == NULL ) {
			strZcpy(buf, SubstrOutBlank((char *)*(long *)lpOpnd[0].values,\
				0, lpOpnd[0].length), 256);
			free((char *)*(long *)lpOpnd[0].values);
		} else {
			strZcpy(buf, SubstrOutBlank((char *)*(long *)lpOpnd[0].oval,\
				0, lpOpnd[0].length), 256);
		}
    } else {
	if( lpOpnd[0].oval == NULL ) {
		strZcpy(buf, (char *)lpOpnd[0].values, 256);
	} else {
		strZcpy(buf, (char *)lpOpnd[0].oval, 256);
	}
    }

    memset(&lpOpnd[0], 0, sizeof(OpndType));
    if( strchr(buf, '.') == NULL ) {
	*(long *)lpOpnd[0].values = atol(buf);
	lpOpnd[0].type = LONG_TYPE;
    } else {
	*(double *)lpOpnd[0].values = atof(buf);
	lpOpnd[0].type = FLOAT_TYPE;
    }

    *OpndTop -= ParaNum - 1;
    return( 0 );

} /* end of function _xVal() */



/*------------
 !                      _xStr()
 !-----------------------------------------------------------------------*/
short PD_style _xStr( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    long   l;
    double f;
    char buf[128];

    switch( lpOpnd[0].type ) {
	case CHR_TYPE:
		l = *(char *)(lpOpnd[0].values);
		ltoa(l, buf, 10);
		break;
	case INT_TYPE:
		l = *(short *)(lpOpnd[0].values);
		ltoa(l, buf, 10);
		break;
	case LONG_TYPE:
		l = *(long *)(lpOpnd[0].values);
		ltoa(l, buf, 10);
		break;
	case FLOAT_TYPE:
		f = *(double *)(lpOpnd[0].values);
		if( ParaNum < 2 ) {
		   sprintf(buf, "%.2f", f);
		} else {
		   char fmt[64];
		   l=xGetOpndLong(&lpOpnd[1]);
		   if( l == LONG_MIN )
		       l = 4;
		   sprintf(fmt, "%%.%df", l);
		   sprintf(buf, fmt, f);
		}
		break;
	default:
	{
	     char *sz = xGetOpndString(&lpOpnd[0]);
	     if( sz == NULL ) {
		buf[0] = '\0';
	     } else {
		strZcpy(buf, sz, MAX_OPND_LENGTH);
	     }
	}
    }

    lpOpnd[0].oval = NULL;
    lpOpnd[0].length = strlen(strZcpy(lpOpnd[0].values, buf, MAX_OPND_LENGTH));
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;
    return( 0 );

} /* end of function _xStr() */


/*------------
 !                       _ASK_Locate
 ! protocol:  locate(char *dbfOrNdxName, char *keyOrExpr...);
 ! locate(dbf, "e_expr=", code) -> locate(dbf, "e_expr=\"code\"")
 !-----------------------------------------------------------------------*/
// #pragma argsused	/* Remed by NiuJingyu */
short PD_style _xLocate(OpndType *lpOpnd, short ParaNum, short *OpndTop)
{
    char   buf[4096];
    long   result = -1;
    int    i;

    if( lpOpnd[0].oval == NULL ) {
	if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
		strZcpy(buf, (char *)*(long *)lpOpnd[0].values, 256);
		free( (char *)*(long *)lpOpnd[0].values );
	} else {
		strZcpy(buf, (char *)lpOpnd[0].values, 256);
	}
    } else {
	strZcpy(buf, (char *)lpOpnd[0].oval, 256);
    }

#ifdef WSToMT
    if( lServerAsRunning ) {
	if( buf[1] != ':' ) {
	    char temp[256];
	    strcpy(temp, buf);
	    strcpy(buf, asqlEnv.szAsqlFromPath);
	    strcat(buf, temp);
	}
    } else
#endif
    {
	makefilename(buf, asqlEnv.szAsqlFromPath, buf);
    }

    stoUpper(buf);
    if( strstr(buf, ".NDX") == NULL ) {
	MidCodeType *mcp;
	dFILE  	*df;
	dFILEENV env;

	df = dAwake(buf, (short)(O_RDWR|O_BINARY), (short)SH_DENYNO, (short)(S_IREAD|S_IWRITE));
	if( df != NULL ) {
		saveDfileEnv(df, &env);
		buf[0] = '\0';
		for(i = 1;  i < ParaNum;  i++) {
		    switch( lpOpnd[i].type ) {
		      case DFIELD_IDEN:
			strcat(buf, rtrim(subcopy((char *)*(long *)lpOpnd[i].oval,\
				0, lpOpnd[i].length)) );
			strcat(buf, "T");
			break;
		      case DATE_TYPE:
			strcat(buf, lpOpnd[i].values);
			strcat(buf, "T");
			break;
		      default:
		      {
			char *sp;

			sp=xGetOpndString(&lpOpnd[i]);
			if( sp == NULL )
			    break;

			strcat(buf, "\"");
			strcat(buf, sp);
			strcat(buf, "\"");
		      }
		  }
		}
		mcp = WordAnalyse(buf);
		if( mcp != NULL ) {
			if( SymbolRegister(mcp, df, NULL, 0, NULL, 0) == NULL ) {

				XEXP_ENV xenv;

				saveXexpEnv( &xenv );
				result = DbfLocateExpr(df, mcp);
				restoreXexpEnv( &xenv);
			}
			FreeCode(mcp);
		}
		if( dSleep(df) > 0 ) {
			setDfileEnv(df, &env);
		}
	}
    } else {
	bHEAD *bh;
	bh = IndexOpen(NULL, buf, BTREE_FOR_ITSELF);
	if( bh != NULL ) {
		result = -1;
		if( lpOpnd[1].oval == NULL ) {
		    if( lpOpnd[1].length >= MAX_OPND_LENGTH ) {
			strZcpy(buf, (char *)*(long *)lpOpnd[1].values, 256);
			free( (char *)*(long *)lpOpnd[1].values );
		    } else {
			strZcpy(buf, (char *)lpOpnd[1].values, 256);
		    }
		} else {
		    strZcpy(buf, (char *)lpOpnd[1].oval, 256);
		}
		result = IndexSeek(bh, buf);
		IndexClose(bh);
	}
    }
    *(long *)lpOpnd[0].values = result;
    lpOpnd[0].type = LONG_TYPE;

    *OpndTop -= ParaNum - 1;

    return  0;

} //end of _xLocate()


/*------------
 !                      _xAt()
 !-----------------------------------------------------------------------*/
short PD_style _xAt( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
     unsigned char *s1, *s2, *s;
     char  temp[FIELDMAXLEN+2], temp2[FIELDMAXLEN+2];

     s1 = strZcpy(temp, xGetOpndString(&lpOpnd[0]), FIELDMAXLEN);
     s2 = strZcpy(temp2, xGetOpndString(&lpOpnd[1]), FIELDMAXLEN);

     /*
     if( lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN ) {
	s1 = strZcpy(temp, (char *)*(long *)lpOpnd[0].oval, lpOpnd[0].length+1);
     } else {
	if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
		if( lpOpnd[0].oval == NULL ) {
			f0 = 1;
			s1 = (char *)*(long *)lpOpnd[0].values;
		} else {
			s1 = (char *)lpOpnd[0].oval;
		}
	} else {
		if( lpOpnd[0].oval == NULL ) {
			s1 = (char *)lpOpnd[0].values;
		} else {
			s1 = lpOpnd[0].oval;
		}
	}
     }
     if( lpOpnd[1].type > FIELD_IDEN && lpOpnd[1].type <= SFIELD_IDEN ) {
	s2 = strZcpy(temp2, (char *)*(long *)lpOpnd[1].oval, lpOpnd[1].length+1);
     } else {
	if( lpOpnd[1].length >= MAX_OPND_LENGTH ) {
		if( lpOpnd[1].oval == NULL ) {
			f1 = 1;
			s2 = (char *)*(long *)lpOpnd[1].values;
		} else {
			s2 = (char *)lpOpnd[1].oval;
		}
	} else {
		if( lpOpnd[1].oval == NULL ) {
			s2 = lpOpnd[1].values;
		} else {
			s2 = (char *)lpOpnd[1].oval;
		}
	}
     }
     */

     s = strstr(s2, s1);
     if( s != NULL ) {
	*(long *)lpOpnd[0].values = s - s2 + 1;
     } else {
	*(long *)lpOpnd[0].values = 0;
     }

     lpOpnd[0].type = LONG_TYPE;
     *OpndTop -= ParaNum - 1;
     return  0;

} /* end of function _xAt() */


/*------------
 !                      _xInt()
 !-----------------------------------------------------------------------*/
short PD_style _xInt( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    long temp_long;

    switch( lpOpnd[0].type ) {
	case CHR_TYPE:
		temp_long = (long)*(char *)lpOpnd[0].values;
		break;
	case INT_TYPE:
		temp_long = (long)*(short *)lpOpnd[0].values;
		break;
	case LONG_TYPE:
		temp_long = *(long *)lpOpnd[0].values;
		break;
	case FLOAT_TYPE:
		temp_long = (long)*(double *)lpOpnd[0].values;
		break;
	case STRING_TYPE:
		temp_long = atol(lpOpnd[0].values);
		break;
	case CFIELD_IDEN:
	case NFIELD_IDEN:       /* number Field */
	case FFIELD_IDEN:       /* float Field */
		temp_long = atol(subcopy((char *)*(long *)lpOpnd[0].oval, 0,
					lpOpnd[0].length));
		break;
	default:
		ErrorSet.xERROR = iBadUseOPr;         //* type error
		return  -10003;
    }

    lpOpnd[0].type = LONG_TYPE;
    *(long *)lpOpnd[0].values = temp_long;

    *OpndTop -= ParaNum - 1;
    return( 0 );

} /* end of function _xInt() */


/*------------
 !                      _xRound()
 !-----------------------------------------------------------------------*/
short PD_style _xRound( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    long   temp_long;
    double temp_double, d;

    temp_double = xGetOpndFloat( &lpOpnd[0] );

    d = floor(temp_double);
    if( temp_double - d > 0.5 )
	temp_long = (long)(d + 1.0);
    else
	temp_long = (long)d;

    lpOpnd[0].type = LONG_TYPE;
    *(long *)lpOpnd[0].values = temp_long;

    *OpndTop -= ParaNum - 1;
    return( 0 );

} /* end of function _xRound() */


/*------------
 !                      _xCeil()
 !-----------------------------------------------------------------------*/
short PD_style _xCeil( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    long   temp_long;
    double temp_double;

    temp_double = xGetOpndFloat( &lpOpnd[0] );

    temp_long = (long)ceil(temp_double);

    lpOpnd[0].type = LONG_TYPE;
    *(long *)lpOpnd[0].values = temp_long;

    *OpndTop -= ParaNum - 1;
    return( 0 );

} /* end of function _xCeil() */


/*------------
 !                      _xFloor()
 !-----------------------------------------------------------------------*/
short PD_style _xFloor( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    long   temp_long;
    double temp_double;

    temp_double = xGetOpndFloat( &lpOpnd[0] );

    temp_long = (long)floor(temp_double);

    lpOpnd[0].type = LONG_TYPE;
    *(long *)lpOpnd[0].values = temp_long;

    *OpndTop -= ParaNum - 1;
    return( 0 );

} /* end of function _xFloor() */


/*------------
 !                      _xSqrt()
 !-----------------------------------------------------------------------*/
short PD_style _xSqrt( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    double temp_double;

    switch( lpOpnd[0].type ) {
	case CHR_TYPE:
		temp_double = *(char *)lpOpnd[0].values;
		break;
	case INT_TYPE:
		temp_double = *(short *)lpOpnd[0].values;
		break;
	case LONG_TYPE:
		temp_double = *(long *)lpOpnd[0].values;
		break;
	case FLOAT_TYPE:
		temp_double = *(double *)lpOpnd[0].values;
		break;
	case STRING_TYPE:
		temp_double = atof(lpOpnd[0].values);
		break;
	case CFIELD_IDEN:
	case NFIELD_IDEN:       /* number Field */
	case FFIELD_IDEN:       /* float Field */
		temp_double = atof(subcopy((char *)*(long *)lpOpnd[0].oval, 0,
					lpOpnd[0].length));
		break;
	default:
		ErrorSet.xERROR = iBadUseOPr;         //* type error
		return( -10003 );
    }

    lpOpnd[0].type = FLOAT_TYPE;

    if( temp_double > 0 )
	*(double *)lpOpnd[0].values = sqrt(temp_double);
    else
	*(double *)lpOpnd[0].values = 0.0;

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xSqrt() */


/*------------
 !                      _xExp()
 !-----------------------------------------------------------------------*/
short PD_style _xExp( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    double temp_double;

    switch( lpOpnd[0].type ) {
	case CHR_TYPE:
		temp_double = *(char *)lpOpnd[0].values;
		break;
	case INT_TYPE:
		temp_double = *(short *)lpOpnd[0].values;
		break;
	case LONG_TYPE:
		temp_double = *(long *)lpOpnd[0].values;
		break;
	case FLOAT_TYPE:
		temp_double = *(double *)lpOpnd[0].values;
		break;
	case STRING_TYPE:
		temp_double = atof(lpOpnd[0].values);
		break;
	case CFIELD_IDEN:
	case NFIELD_IDEN:       /* number Field */
	case FFIELD_IDEN:       /* float Field */
		temp_double = atof(subcopy((char *)*(long *)lpOpnd[0].oval, 0,
					lpOpnd[0].length));
		break;
	default:
		ErrorSet.xERROR = iBadUseOPr;         //* type error
		return( -10003 );
    }

    lpOpnd[0].type = FLOAT_TYPE;
    *(double *)lpOpnd[0].values = exp(temp_double);

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xExp() */


/*------------
 !                      _xLog()
 !-----------------------------------------------------------------------*/
short PD_style _xLog( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    double temp_double;

    switch( lpOpnd[0].type ) {
	case CHR_TYPE:
		temp_double = *(char *)lpOpnd[0].values;
		break;
	case INT_TYPE:
		temp_double = *(short *)lpOpnd[0].values;
		break;
	case LONG_TYPE:
		temp_double = *(long *)lpOpnd[0].values;
		break;
	case FLOAT_TYPE:
		temp_double = *(double *)lpOpnd[0].values;
		break;
	case STRING_TYPE:
		temp_double = atof(lpOpnd[0].values);
		break;
	case CFIELD_IDEN:
	case NFIELD_IDEN:       /* number Field */
	case FFIELD_IDEN:       /* float Field */
		temp_double = atof(subcopy((char *)*(long *)lpOpnd[0].oval, 0,
					lpOpnd[0].length));
		break;
	default:
		ErrorSet.xERROR = iBadUseOPr;         //* type error
		return( -10003 );
    }

    lpOpnd[0].type = FLOAT_TYPE;
    *(double *)lpOpnd[0].values = log(temp_double);

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xLog() */


/*------------
 !                      _xTrim()
 !-----------------------------------------------------------------------*/
short PD_style _xTrim( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    char  *s;
    short i;

    //use substr to hold the temp string
    if( lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN ) {
	s = subcopy((char *)*(long *)lpOpnd[0].oval, 0, lpOpnd[0].length);
    } else {
	if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
		if( lpOpnd[0].oval == NULL ) {
			s = SubstrOutBlank((char *)*(long *)lpOpnd[0].values,\
							0, lpOpnd[0].length );
			free((char *)*(long *)lpOpnd[0].values);
		} else {
			s = SubstrOutBlank(lpOpnd[0].oval,0,lpOpnd[0].length);
		}
	} else {
		if( lpOpnd[0].oval == NULL ) {
			s = lpOpnd[0].values;
		} else {
			s = (char *)lpOpnd[0].oval;
		}
	}
    }

    if( (i = strlen(trim(s)) ) >= MAX_OPND_LENGTH ) {
	*(long *)(lpOpnd[0].values) = (long)strdup(s);
    } else {
	strcpy(lpOpnd[0].values, s);
    }

    lpOpnd[0].oval = NULL;
    lpOpnd[0].length = i;
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xTrim() */


/*------------
 !                      _xLtrim()
 !-----------------------------------------------------------------------*/
short PD_style _xLtrim( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    char  *s;
    short i;

    //use substr to hold the temp string
    if( lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN ) {
	s = subcopy((char *)*(long *)lpOpnd[0].oval, 0, lpOpnd[0].length);
    } else {
	if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
		if( lpOpnd[0].oval == NULL ) {
			s = SubstrOutBlank((char *)*(long *)lpOpnd[0].values,\
							0, lpOpnd[0].length );
			free((char *)*(long *)lpOpnd[0].values);
		} else {
			s = SubstrOutBlank(lpOpnd[0].oval,0,lpOpnd[0].length);
		}
	} else {
		if( lpOpnd[0].oval == NULL ) {
			s = lpOpnd[0].values;
		} else {
			s = (char *)lpOpnd[0].oval;
		}
	}
    }

    if( (i = strlen(ltrim(s)) ) >= MAX_OPND_LENGTH ) {
	*(long *)(lpOpnd[0].values) = (long)strdup(s);
    } else {
	strcpy(lpOpnd[0].values, s);
    }


    lpOpnd[0].oval = NULL;
    lpOpnd[0].length = i;
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xLtrim() */


/*------------
 !                      _xRtrim()
 !-----------------------------------------------------------------------*/
short PD_style _xRtrim( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    char  *s;
    short i;

    //use substr to hold the temp string
    if( lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN ) {
	s = subcopy((char *)*(long *)lpOpnd[0].oval, 0, lpOpnd[0].length);
    } else {
	if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
		if( lpOpnd[0].oval == NULL ) {
			s = SubstrOutBlank((char *)*(long *)lpOpnd[0].values,\
							0, lpOpnd[0].length );
			free((char *)*(long *)lpOpnd[0].values);
		} else {
			s = SubstrOutBlank(lpOpnd[0].oval,0,lpOpnd[0].length);
		}
	} else {
		if( lpOpnd[0].oval == NULL ) {
			s = lpOpnd[0].values;
		} else {
			s = (char *)lpOpnd[0].oval;
		}
	}
    }

    if( (i = strlen(rtrim(s)) ) >= MAX_OPND_LENGTH ) {
	*(long *)(lpOpnd[0].values) = (long)strdup(s);
    } else {
	strcpy(lpOpnd[0].values, s);
    }


    lpOpnd[0].oval = NULL;
    lpOpnd[0].length = i;
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xRtrim() */


/*------------
 !                      _xUpper()
 !-----------------------------------------------------------------------*/
short PD_style _xUpper( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    char  *s;
    short i;

    //use substr to hold the temp string
    if( lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN ) {
	s = subcopy((char *)*(long *)lpOpnd[0].oval, 0, lpOpnd[0].length);
    } else {
	if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
		if( lpOpnd[0].oval == NULL ) {
			s = SubstrOutBlank((char *)*(long *)lpOpnd[0].values,\
							0, lpOpnd[0].length );
			free((char *)*(long *)lpOpnd[0].values);
		} else {
			s = SubstrOutBlank(lpOpnd[0].oval,0,lpOpnd[0].length);
		}
	} else {
		if( lpOpnd[0].oval == NULL ) {
			s = lpOpnd[0].values;
		} else {
			s = (char *)lpOpnd[0].oval;
		}
	}
    }

    if( (i = strlen(stoUpper(s)) ) >= MAX_OPND_LENGTH ) {
	*(long *)(lpOpnd[0].values) = (long)strdup(s);
    } else {
	strcpy(lpOpnd[0].values, s);
    }


    lpOpnd[0].oval = NULL;
    lpOpnd[0].length = i;
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xUpper() */



/*------------
 !                      _xLower()
 !-----------------------------------------------------------------------*/
short PD_style _xLower( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    char  *s;
    short i;

    //use substr to hold the temp string
    if( lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN ) {
	s = subcopy((char *)*(long *)lpOpnd[0].oval, 0, lpOpnd[0].length);
    } else {
	if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
		if( lpOpnd[0].oval == NULL ) {
			s = SubstrOutBlank((char *)*(long *)lpOpnd[0].values,\
							0, lpOpnd[0].length );
			free((char *)*(long *)lpOpnd[0].values);
		} else {
			s = SubstrOutBlank(lpOpnd[0].oval,0,lpOpnd[0].length);
		}
	} else {
		if( lpOpnd[0].oval == NULL ) {
			s = lpOpnd[0].values;
		} else {
			s = (char *)lpOpnd[0].oval;
		}
	}
    }

    if( (i = strlen(stoLower(s)) ) >= MAX_OPND_LENGTH ) {
	*(long *)(lpOpnd[0].values) = (long)strdup(s);
    } else {
	strcpy(lpOpnd[0].values, s);
    }


    lpOpnd[0].oval = NULL;
    lpOpnd[0].length = i;
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xLower() */



/*------------
 !                      _xShrink()
 !-----------------------------------------------------------------------*/
short PD_style _xShrink( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    char  *s;
    short i;

    //use substr to hold the temp string
    if( lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN ) {
	s = subcopy((char *)*(long *)lpOpnd[0].oval, 0, lpOpnd[0].length);
    } else {
	if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
		if( lpOpnd[0].oval == NULL ) {
			s = SubstrOutBlank((char *)*(long *)lpOpnd[0].values,\
							0, lpOpnd[0].length );
			free((char *)*(long *)lpOpnd[0].values);
		} else {
			s = SubstrOutBlank(lpOpnd[0].oval,0,lpOpnd[0].length);
		}
	} else {
		if( lpOpnd[0].oval == NULL ) {
			s = lpOpnd[0].values;
		} else {
			s = (char *)lpOpnd[0].oval;
		}
	}
    }

    if( (i = strlen(shrink(s)) ) >= MAX_OPND_LENGTH ) {
	*(long *)(lpOpnd[0].values) = (long)strdup(s);
    } else {
	strcpy(lpOpnd[0].values, s);
    }


    lpOpnd[0].oval = NULL;
    lpOpnd[0].length = i;
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xShrink() */


/*------------
 !                      _xSpace()
 !-----------------------------------------------------------------------*/
short PD_style _xSpace( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    long temp_long;
    char *s;

    switch( lpOpnd[0].type ) {
	case CHR_TYPE:
		temp_long = *(char *)lpOpnd[0].values;
		break;
	case INT_TYPE:
		temp_long = *(short *)lpOpnd[0].values;
		break;
	case LONG_TYPE:
		temp_long = *(long *)lpOpnd[0].values;
		break;
	case FLOAT_TYPE:
		temp_long = (long)*(double *)lpOpnd[0].values;
		break;
	case STRING_TYPE:
		temp_long = atol(lpOpnd[0].values);
		break;
	case CFIELD_IDEN:
	case NFIELD_IDEN:       /* number Field */
	case FFIELD_IDEN:       /* float Field */
		temp_long = atol(subcopy((char *)*(long *)lpOpnd[0].oval, 0,
					lpOpnd[0].length));
		break;
	default:
		ErrorSet.xERROR = iBadUseOPr;         //* type error
		return( -10003 );
    }

    if( temp_long >= MAX_OPND_LENGTH ) {
	s = malloc(temp_long+1);
	memset(s, ' ', temp_long);
	*(long *)(lpOpnd[0].values) = (long)s;
    } else {
	memset(lpOpnd[0].values, ' ', temp_long);
	lpOpnd[0].values[temp_long] = '\0';
    }

    lpOpnd[0].oval = NULL;
    lpOpnd[0].length = (short)temp_long;
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xSpace() */


/*------------
 !                      _xFile()
 ! protocol:
 !    file(szfileName)
 !-----------------------------------------------------------------------*/
short PD_style _xFile( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    char  *s;
    char   buf[MAXPATH];

    //use substr to hold the temp string
    s = xGetOpndString(&lpOpnd[0]);
    if( s == NULL || *s == '\0' )
    { //to avoid error
	s = "\0\0";
    }

#ifdef WSToMT
    if( lServerAsRunning ) {
	if( s[1] == ':' ) {
	    strZcpy(buf, s, MAXPATH);
	} else {
	    if( *s != '^' ) {
		strcpy(buf, asqlEnv.szAsqlFromPath);
		strncat(buf, s, MAXPATH-strlen(buf));
	    } else {
		strcpy(buf, asqlEnv.szAsqlResultPath);
		strncat(buf, s+1, MAXPATH-strlen(buf));
	    }
	}
    } else
#endif
    {
	if( *s != '^' ) {
	    makefilename(buf, asqlEnv.szAsqlFromPath, s);
	} else {
	    makefilename(buf, asqlEnv.szAsqlFromPath, s+1);
	}
    }

    //exist: "R" or "W" or "", nonexist "N"
    if( access(buf, 0) == 0 ) {
	lpOpnd[0].length = 0;
	lpOpnd[0].values[0] = '\0';
	if( access(buf, 4) == 0 ) {
		strcpy(lpOpnd[0].values, "R");
		lpOpnd[0].length = 1;
	}
	if( access(buf, 2) == 0 ) {
		strcat(lpOpnd[0].values, "W");
		(lpOpnd[0].length)++;
	}
    } else {
	strcpy(lpOpnd[0].values, "N");
	lpOpnd[0].length = 1;
    }

    lpOpnd[0].oval = NULL;
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xFile() */


/*------------
 !                      _xHzth()
 ! protocol:
 !    hzth(code, hzthMethod, way)
 ! example:
 !    hzth("010111", "EC_M8")
 !    hzth("010111", "%BIRTHDAY")
 !-----------------------------------------------------------------------*/
short PD_style _xHzth( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    char  *s;
    char  code[FIELDMAXLEN];
    char  method[FIELDMAXLEN];
    HZTH_METHOD_STRU *hms;

    if( getHzthInitFlag() != 1 ) {
	ErrorSet.xERROR = iHZTHnoInit;         //*not initialize
	return  -iHZTHnoInit;
    }

    if( lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN ) {
	s = subcopy((char *)*(long *)lpOpnd[0].oval, 0, lpOpnd[0].length);

	if( ParaNum < 2 ) {

		dFIELDWHENACTION *dfld = (dFIELDWHENACTION *)(lpOpnd[0].oval);

		strZcpy(code, s, FIELDMAXLEN);
		strcpy(&method[1], getFieldInfo(dfld->pSourceDfile,\
						    dfld->wSourceid)->field);
		goto xHZTH_JMP;
	}

    } else {
	if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
		if( lpOpnd[0].oval == NULL ) {
			s = SubstrOutBlank((char *)*(long *)lpOpnd[0].values,\
							0, lpOpnd[0].length );
			free((char *)*(long *)lpOpnd[0].values);
		} else {
			s = SubstrOutBlank(lpOpnd[0].oval,0,lpOpnd[0].length);
		}
	} else {
		if( lpOpnd[0].oval == NULL ) {
			s = lpOpnd[0].values;
		} else {
			s = (char *)lpOpnd[0].oval;
		}
	}
    }
    strZcpy(code, s, FIELDMAXLEN);

    if( lpOpnd[1].type >= FIELD_IDEN && lpOpnd[1].type <= SFIELD_IDEN ) {
	s = subcopy((char *)*(long *)lpOpnd[1].oval, 0, lpOpnd[1].length);
    } else {
	if( lpOpnd[1].length >= MAX_OPND_LENGTH ) {
		if( lpOpnd[1].oval == NULL ) {
			s = SubstrOutBlank((char *)*(long *)lpOpnd[1].values,\
							0, lpOpnd[1].length );
			free((char *)*(long *)lpOpnd[1].values);
		} else {
			s = SubstrOutBlank(lpOpnd[1].oval,0,lpOpnd[1].length);
		}
	} else {
		if( lpOpnd[1].oval == NULL ) {
			s = lpOpnd[1].values;
		} else {
			s = (char *)lpOpnd[1].oval;
		}
	}
    }
    strZcpy(method, s, FIELDMAXLEN);

    if( ltrim(method)[0] == '%' ) 
    { 
	//2000.5.18 Xilong, TID support
	//hzth(sex, "100@SEX")
	char   *seg, *macroName;
	//char   *label, *key;

xHZTH_JMP:
	
	macroName = &method[1];
	s = strchr(macroName, '.');
	if( s != NULL ) {
	    *s++ = '\0';
	    seg = macroName;
	} else {
	    seg = NULL;
	    s = macroName;
	}

	/*key = strchr(s, ':');
	if( key != NULL ) {
	    *key++ = '\0';
	    label = s;
	} else {
	    key = s;
	    label = NULL;
	}*/

	//s = GetCfgKey(csuDataDictionary, "FIELDCTL", &method[1], "REPDESCRIP");
	s = GetCfgKey(csuDataDictionary, seg, s, "REPDESCRIP");
	if( s == NULL ) {
		s = code;
		goto XEXPFUN_JMP1;
	}
	strZcpy(method, s, FIELDMAXLEN);
    }

    hms = hMethodTranslate(method);
    if( hms == NULL ) {
	s = code;
    } else {
	if( ParaNum > 3 )
	{ //ext replace
	    s = hExtReplaceByMethod(hms, code);
	} else {
	    s = hReplaceByMethod(hms, code);
	}
	if( s == NULL )
	    s = code;
    }

    if( hms != NULL )	free(hms);

XEXPFUN_JMP1:
    lpOpnd[0].length = strlen(s);
    if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
	*(long *)(lpOpnd[0].values) = (long)strdup(s);
    } else {
	strcpy(lpOpnd[0].values, s);
    }

    lpOpnd[0].oval = NULL;
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xHzth() */


/*------------
 !                      _xArrayInit()
 !-----------------------------------------------------------------------*/
// #pragma argsused		/* Remed by NiuJingyu */
short PD_style _xArrayInit(OpndType *lpOpnd, short ParaNum, short *OpndTop)
{
    unsigned short i;
    ArrayType      *p;
    long 	   MemSize;

    for( i = 0;   i < ParaNum;   i++ ) {

	if( lpOpnd[i].type != ARRAY_TYPE )      continue;

		//oval will never be NULL, for our define for ARRAY_TYPE
		MemSize = (p = (ArrayType *)*(long *)lpOpnd[i].oval)->MemSize;
		memset(p->ArrayMem, 0, MemSize);        // init the memory

    } /* end of for(i) */

    *OpndTop -= ParaNum;                /* maintain the opnd stack */

    return 0;

} /* end of function _xArrayInit() */


/*------------
 !                      _xInputStr()
 ! protocol:
 !    inputstr(label, initVal)
 !-----------------------------------------------------------------------*/
short PD_style _xInputStr( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
#ifdef __N_C_S
    char  *s;
    char  buf[256];

    if( ParaNum > 1 ) {
    if( lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN ) {
	s = SubstrOutBlank((char *)*(long *)lpOpnd[0].oval, 0, lpOpnd[0].length);
    } else {
	if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
		if( lpOpnd[0].oval == NULL ) {
			s = SubstrOutBlank((char *)*(long *)lpOpnd[0].values,\
							0, lpOpnd[0].length );
			free((char *)*(long *)lpOpnd[0].values);
		} else {
			s = SubstrOutBlank(lpOpnd[0].oval,0,lpOpnd[0].length);
		}
	} else {
		if( lpOpnd[0].oval == NULL ) {
			s = lpOpnd[0].values;
		} else {
			s = (char *)lpOpnd[0].oval;
		}
	}
    }
    strZcpy(buf, s, 256);
    } else {
    buf[0] = '\0';
    }

    //use substr to hold the temp string
    if( lpOpnd[0].type >= FIELD_IDEN && lpOpnd[0].type <= SFIELD_IDEN ) {
	s = substr((char *)*(long *)lpOpnd[0].oval, 0, lpOpnd[0].length);
    } else {
	if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
		if( lpOpnd[0].oval == NULL ) {
			s = substr((char *)*(long *)lpOpnd[0].values,\
							0, lpOpnd[0].length );
			free((char *)*(long *)lpOpnd[0].values);
		} else {
			s = substr(lpOpnd[0].oval,0,lpOpnd[0].length);
		}
	} else {
		if( lpOpnd[0].oval == NULL ) {
			s = lpOpnd[0].values;
		} else {
			s = (char *)lpOpnd[0].oval;
		}
	}
    }

    if( inputBox("������", s, buf, 256) != cmOK ) {
	lpOpnd[0].values[0] = '\0';
	lpOpnd[0].length = 0;
    }
    if( (lpOpnd[0].length = strlen(buf)) < MAX_OPND_LENGTH ) {
	strcpy(lpOpnd[0].values, buf);
    } else {
	*(long *)lpOpnd[0].values = (long)strdup(buf);
    }

#endif

    lpOpnd[0].oval = NULL;
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xInputStr() */



/*------------
 !                      _xSysVar()
 ! protocol:
 !    sysvar([r/w],[fileName], macroName)
 ! group@label:key,cont
 !-----------------------------------------------------------------------*/
short PD_style _xSysVar( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    char   *s;
    char    fileName[MAXPATH];
    char    macroName[128];
    char    buf[256];
    char   *seg, *label, *key, *cont;
    double  f;
    short   wParaUsed;
    PCFG_STRU cfgHandle;
    char      c;

    if( lpOpnd[0].type == CHR_TYPE ) {
	wParaUsed = 1;
    } else {
	wParaUsed = 0;
    }

    if( ParaNum - wParaUsed < 1 ) {
	ErrorSet.xERROR = iSysvarParaErr;
	return  -iSysvarParaErr;
    }


    if( ParaNum - wParaUsed < 2 ) {

	cfgHandle = csuDataDictionary;

	s = xGetOpndString(&lpOpnd[wParaUsed]);
	strZcpy(macroName, s, 128);
    } else {
	s = xGetOpndString(&lpOpnd[wParaUsed]);
	strZcpy(fileName, s, MAXPATH);
	cfgHandle = OpenCfgFile(fileName);
	if( cfgHandle == NULL ) {
		*OpndTop -= ParaNum - 1;
		return  1;
	}

	s = xGetOpndString(&lpOpnd[1+wParaUsed]);
	strZcpy(macroName, s, 128);
    }

    s = strchr(macroName, '@');
    if( s != NULL ) {
	*s++ = '\0';
	seg = macroName;
    } else {
	seg = NULL;
	s = macroName;
    }

    key = strchr(s, ':');
    if( key != NULL ) {
	*key++ = '\0';
	label = s;
    } else {
	key = s;
	label = NULL;
    }

    if( stricmp(seg, "SERIALNO") == 0 ) {

	EnterCriticalSection( &(cfgHandle->dCriticalSection) );

	s = GetCfgKey(cfgHandle, seg, label, key);
	if( s == NULL )
		s = "";

	f = atof(s);
	sprintf(buf, "%.0f", f+1);
	WriteCfgKey(cfgHandle, seg, label, key, buf);
	*(double *)lpOpnd[0].values = f;
	lpOpnd[0].type = FLOAT_TYPE;

	LeaveCriticalSection( &(cfgHandle->dCriticalSection) );

    } else {

	if( wParaUsed )
	    c = toupper(xGetOpndChr(&lpOpnd[0]));

	lpOpnd[0].type = STRING_TYPE;
	if( wParaUsed == 0 || c == 'R' ) {
	    s = GetCfgKey(cfgHandle, seg, label, key);
	    if( s == NULL )
		s = "";

	    if( (lpOpnd[0].length = strlen(s)) < MAX_OPND_LENGTH ) {
		strcpy(lpOpnd[0].values, s);
	    } else {
		*(long *)lpOpnd[0].values = (long)strdup(s);
	    }
	} else if( c == 'W' ) {
	    cont = strchr(key, ',');
	    if( cont == NULL || seg == NULL) {
		lpOpnd[0].length = 1;
		strcpy(lpOpnd[0].values, "1");
	    } else {

		if( *label == '`' )
		    label = NULL;

		*cont = '\0';
		WriteCfgKey(cfgHandle, seg, label, key, cont+1);
		lpOpnd[0].length = 1;
		strcpy(lpOpnd[0].values, "");
	    }
	} else {
	    char buf[4096];
	    int  iBufSize = 4096;

	    if( seg != NULL && *seg == '`' )
		seg = NULL;

	    if( label != NULL && *label == '`' )
		label = NULL;

	    if( key != NULL && *key == '`' )
		key = NULL;

	    enumItemInCache(cfgHandle, seg, label, key, buf, &iBufSize);
	    if( (lpOpnd[0].length = strlen(buf)) < MAX_OPND_LENGTH ) {
		strcpy(lpOpnd[0].values, buf);
	    } else {
		*(long *)lpOpnd[0].values = (long)strdup(buf);
	    }
	}
    }
    lpOpnd[0].oval = NULL;

    if( ParaNum - wParaUsed >= 2 ) {
	CloseCfgFile(cfgHandle);
    }

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xSysVar() */







/*------------
 !                      _xDbfVal()
 ! protocol:  dbfval(char *dbfOrNdxName, char *fieldName, char *keyOrExpr...);
 ! dbfval(dbf, name, "e_expr=", code) -> dbfval(dbf, name, "e_expr=\"code\"")
 !---------------------------------------------------------___--------------*/
// #pragma argsused		/* Remed by NiuJingyu */
short PD_style _xDbfVal(OpndType *lpOpnd, short ParaNum, short *OpndTop)
{
    char   buf[1024];
    long   result;
    int    i;
    char   *s;
    char   fieldName[32];
    unsigned short  fldid;

    if( lpOpnd[1].type >= FIELD_IDEN && lpOpnd[1].type <= SFIELD_IDEN ) {
	s = subcopy((char *)*(long *)lpOpnd[1].oval, 0, lpOpnd[1].length);
    } else {
	if( lpOpnd[1].length >= MAX_OPND_LENGTH ) {
		if( lpOpnd[1].oval == NULL ) {
			s = SubstrOutBlank((char *)*(long *)lpOpnd[1].values,\
							0, lpOpnd[1].length );
			free((char *)*(long *)lpOpnd[1].values);
		} else {
			s = SubstrOutBlank(lpOpnd[1].oval,0,lpOpnd[1].length);
		}
	} else {
		if( lpOpnd[1].oval == NULL ) {
			s = lpOpnd[1].values;
		} else {
			s = (char *)lpOpnd[1].oval;
		}
	}
    }
    strZcpy(fieldName, s, 32);

    if( lpOpnd[0].oval == NULL ) {
	if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
		strZcpy(buf, (char *)*(long *)lpOpnd[0].values, 256);
		free( (char *)*(long *)lpOpnd[0].values );
	} else {
		strZcpy(buf, (char *)lpOpnd[0].values, 256);
	}
    } else {
	strZcpy(buf, (char *)lpOpnd[0].oval, 256);
    }

#ifdef WSToMT
    if( lServerAsRunning ) {
	if( buf[1] != ':' ) {
	    char temp[256];
	    strcpy(temp, buf);
	    strcpy(buf, asqlEnv.szAsqlFromPath);
	    strcat(buf, temp);
	}
    } else
#endif
    {
	makefilename(buf, asqlEnv.szAsqlFromPath, buf);
    }

    stoUpper(buf);

    if( strstr(buf, ".NDX") == NULL ) {
	MidCodeType *mcp;
	dFILE  	*df;
	dFILEENV env;

	df = dAwake(buf, (short)(O_RDWR|O_BINARY), (short)SH_DENYNO, (short)(S_IREAD|S_IWRITE));
	if( df == NULL ) {
		ErrorSet.xERROR = iNoDBF;
		strcpy(ErrorSet.string, "dbfval()");
		return  1;
	} else {
		saveDfileEnv(df, &env);
		buf[0] = '\0';
		for(i = 2;  i < ParaNum;  i++) {
		  if( lpOpnd[i].oval == NULL )
		  { //var string
		    if( lpOpnd[i].length >= MAX_OPND_LENGTH ) {
			strcat(buf, (char *)*(long *)lpOpnd[i].values);
			free( (char *)*(long *)lpOpnd[i].values );
		    } else {
			strcat(buf, (char *)lpOpnd[i].values);
		    }
		  } else {
		    if( lpOpnd[i].type >= FIELD_IDEN && lpOpnd[i].type <= SFIELD_IDEN ) {
			strcat(buf, "\"");
			strcat(buf, rtrim(SubstrOutBlank((char *)*(long *)lpOpnd[i].oval,\
				0, lpOpnd[i].length)) );
			strcat(buf, "\"");
		    } else {
			strcat(buf, (char *)lpOpnd[i].oval);
		    }
		  }
		}
		mcp = WordAnalyse(buf);
		if( mcp != NULL ) {
			if( SymbolRegister(mcp, df, NULL, 0, NULL, 0) == NULL ) {
				//recall xexp will destroy the opnd.
				//here we must save the opnd.
				optrStackFund = optr_top;
				opndStackFund = opnd_top;

				result = DbfLocateExpr(df, mcp);

				optr_top = optrStackFund;
				opnd_top = opndStackFund;
				optrStackFund = 0;
				opndStackFund = 0;

			}
			FreeCode(mcp);
		}
		if( result >= 0 ) {
			fldid = GetFldid(df,fieldName);
			if( fldid != 0xFFFF ) {
			    get_fld(df, fldid, buf);
			} else {
			    buf[0] = '\0';
			}
		} else {
			buf[0] = '\0';
		}

		if( dSleep(df) > 0 ) {
			setDfileEnv(df, &env);
		}
	}
    } else {
	bHEAD *bh;
	bh = IndexOpen(NULL, buf, BTREE_FOR_ITSELF);
	if( bh == NULL ) {
		ErrorSet.xERROR = iNoDBF;
		strcpy(ErrorSet.string, "dbfval()");
		return  1;
	} else {
		if( lpOpnd[2].oval == NULL ) {
		    if( lpOpnd[2].length >= MAX_OPND_LENGTH ) {
			strZcpy(buf, (char *)*(long *)lpOpnd[2].values, 256);
			free( (char *)*(long *)lpOpnd[2].values );
		    } else {
			strZcpy(buf, (char *)lpOpnd[2].values, 256);
		    }
		} else {
		    strZcpy(buf, (char *)lpOpnd[2].oval, 256);
		}
		result = IndexSeek(bh, buf);
		IndexClose(bh);

		if( result >= 0 ) {
			itoa(result, buf, 10);
		} else {
			buf[0] = '\0';
		}
	}
    }

    if( (i = strlen(buf)) >= MAX_OPND_LENGTH ) {
	*(long *)(lpOpnd[0].values) = (long)strdup(buf);
    } else {
	strcpy(lpOpnd[0].values, buf);
    }
    lpOpnd[0].oval = NULL;
    lpOpnd[0].length = i;
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;

    return  0;

} //end of _xDbfVal()


/*------------
 !                      _xRelCnt()
 ! relcnt()
 !-----------------------------------------------------------------------*/
short PD_style _xRelCnt(OpndType *lpOpnd, short ParaNum, short *OpndTop)
{
extern WSToMT FromToStru fFrTo;
    long  l;
    char  *sz;
    bHEAD *bh;
    char   buf[256];

    if( (l = xGetOpndLong(&lpOpnd[0]) ) == LONG_MIN ) {
	return ErrorSet.xERROR = iRelCntParaErr;
    }

    if( ParaNum > 1 ) {
	sz = xGetOpndString(&lpOpnd[1]);
    } else {
	sz = NULL;
    }

    l--;

    if( l < 0 || l >= fFrTo.cSouDbfNum ) {
	return ErrorSet.xERROR = iRelCntParaErr;
    }

    bh = fFrTo.syncBh[l];
    if( bh == NULL ) {
	return ErrorSet.xERROR = iRelCntParaErr;
    }
    bh = bh->pbh;
    if( bh == NULL ) {
	return ErrorSet.xERROR = iRelCntParaErr;
    }

    if( sz == NULL ) {
	l = getRecNum(fFrTo.cSouFName[l]);
    } else {
	strZcpy(buf, sz, 256);
	l = IndexKeyCount(bh, buf);
    }

    *(long *)lpOpnd[0].values = l;
    lpOpnd[0].type = LONG_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xRelCnt() */


/*------------
 !                      _xFmtStr()
 !-----------------------------------------------------------------------*/
short PD_style _xFmtStr(OpndType *lpOpnd, short ParaNum, short *OpndTop)
{
    char buf[4096];
    char *sp;
    char fmtStr[256];

    sp=xGetOpndString(&lpOpnd[0]);
    strZcpy(fmtStr, sp, 256);

    switch( lpOpnd[1].type ) {
	case CHR_TYPE:
		sprintf(buf, fmtStr, *(char *)(lpOpnd[1].values));
		break;
	case INT_TYPE:
		sprintf(buf, fmtStr, *(short *)(lpOpnd[1].values));
		break;
	case LONG_TYPE:
		sprintf(buf, fmtStr, *(long *)(lpOpnd[1].values));
		break;
	case FLOAT_TYPE:
		sprintf(buf, fmtStr, *(double *)(lpOpnd[1].values));
		break;
	default:
		sp=xGetOpndString(&lpOpnd[1]);
		sprintf(buf, fmtStr, sp);
    }

    lpOpnd[0].oval = NULL;
    lpOpnd[0].length = strlen(buf);
    if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
	*(long *)(lpOpnd[0].values) = (long)strdup(buf);
    } else {
	strcpy(lpOpnd[0].values, buf);
    }
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;
    return( 0 );

} /* end of function _xFmtStr() */


/*------------
 !                      _xDecrypt()
 ! tdecrypt()
 !-----------------------------------------------------------------------*/
short PD_style _xDecrypt( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    char  *s, *key;
    short i;

    //use substr to hold the temp string
    s = strdup(xGetOpndString(&lpOpnd[0]));
    key = strdup(xGetOpndString(&lpOpnd[1]));

    if( (i = strlen(s) ) >= MAX_OPND_LENGTH ) {
	*(long *)(lpOpnd[0].values) = (long)malloc(i+1);
	_DES(key, s, (char *)*(long *)(lpOpnd[0].values));
    } else {
	_DES(key, s, lpOpnd[0].values);
    }

    free(s);
    free(key);

    lpOpnd[0].oval = NULL;
    lpOpnd[0].length = i;
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xDecrypt() */


/*------------
 !                      _xEncrypt()
 ! tencrypt()
 !-----------------------------------------------------------------------*/
short PD_style _xEncrypt( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    char  *s, *key;
    short i;

    //use substr to hold the temp string
    s = strdup(xGetOpndString(&lpOpnd[0]));
    key = strdup(xGetOpndString(&lpOpnd[1]));

    if( (i = strlen(s) ) >= MAX_OPND_LENGTH ) {
	*(long *)(lpOpnd[0].values) = (long)malloc(i+1);
	DES(key, s, (char *)*(long *)(lpOpnd[0].values));
    } else {
	DES(key, s, lpOpnd[0].values);
    }

    free(s);
    free(key);

    lpOpnd[0].oval = NULL;
    lpOpnd[0].length = i;
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xEncrypt() */



/*------------
 !                      _xTStrGet()
 ! tstrget()
 !-----------------------------------------------------------------------*/
short PD_style _xTStrGet( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
     unsigned char *s, *sv;
     char           c;
     int	    i, len;

     s = xGetOpndString( &lpOpnd[0] );
     c = xGetOpndChr( &lpOpnd[1] );
     i = xGetOpndLong( &lpOpnd[2] ) - 1;

     if( i < 0 )
	 s = NULL;
     else
	 s = strnchr(s, strlen(s), c, (short)i);

     if( s != NULL ) {
	sv = ++s;

	while( *s && *s != c )	s++;
	*s = '\0';

	len = s - sv + 1;
	if( len >= MAX_OPND_LENGTH ) {
		s = strdup( s );
		*(long *)(lpOpnd[0].values) = (long)s;
	} else {
		strcpy(lpOpnd[0].values, sv);
	}
	lpOpnd[0].length = len;
     } else {
	lpOpnd[0].values[0] = '\0';
	lpOpnd[0].length = 0;
     }

    lpOpnd[0].oval = NULL;
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} //end of function _xTStrGet()


/*------------
 !                      _xXin()
 ! xin(str, str1, str2, str3)
 !     if( str = str1 )	return 1
 !     if( str = str2 )	return 2
 !-----------------------------------------------------------------------*/
// #pragma argsused		/* Remed by NiuJingyu */
short PD_style _xXin(OpndType *lpOpnd, short ParaNum, short *OpndTop)
{
    unsigned short i, len, k;
    char 	   *sz;
    char	   buf[256];
    register char  c1, c2;

    sz = xGetOpndString(&lpOpnd[0]);
    if( sz != NULL ) {
	len = strlen( strZcpy(buf, sz, 256) );
    } else {
	*OpndTop -= ParaNum;    /* maintain the opnd stack */
	return  1;
    }

    for( k = 1;   k < ParaNum;   k++ ) {
	sz = xGetOpndString(&lpOpnd[k]);
	if( sz == NULL ) {
		*OpndTop -= ParaNum - 1 ;    /* maintain the opnd stack */
		return  1;
	}

	for(i = 0;  i < len;  i++ ) {

		c1 = buf[i];
		c2 = sz[i];

		if( ( c1 != c2  && c1 != '*' && c2 != '*') || \
							    (c2 == '\0') ) {
			break;
		}
	}

	if( i >= len || c2 == '\0' ) {
		*(long *)lpOpnd[0].values = k;
		lpOpnd[0].type = LONG_TYPE;
		lpOpnd[0].length = sizeof(long);

		*OpndTop -= ParaNum - 1 ;    /* maintain the opnd stack */
		return  0;
	}
    }

    *(long *)lpOpnd[0].values = 0;
    lpOpnd[0].type = LONG_TYPE;
    lpOpnd[0].length = sizeof(long);

    *OpndTop -= ParaNum - 1 ;    /* maintain the opnd stack */
    return  0;

}



/*------------
 !                      _xXina()
 ! xina(str, str1, str2, str3)
 !     if( str == str1 )	return 1
 !     if( str == str2 )	return 2
 !-----------------------------------------------------------------------*/
// #pragma argsused		/* Remed by NiuJingyu */
short PD_style _xXina(OpndType *lpOpnd, short ParaNum, short *OpndTop)
{
    unsigned short k;
    char 	   *sz;
    char	   buf[256];

    sz = xGetOpndString(&lpOpnd[0]);
    if( sz != NULL ) {
	strZcpy(buf, sz, 256);
    } else {
	*OpndTop -= ParaNum;    /* maintain the opnd stack */
	return  1;
    }

    for( k = 1;   k < ParaNum;   k++ ) {
	sz = xGetOpndString(&lpOpnd[k]);
	if( sz == NULL ) {
		*OpndTop -= ParaNum - 1 ;    /* maintain the opnd stack */
		return  1;
	}

	if( strcmp(buf, sz) == 0 ) {
		*(long *)lpOpnd[0].values = k;
		lpOpnd[0].type = LONG_TYPE;
		lpOpnd[0].length = sizeof(long);

		*OpndTop -= ParaNum - 1 ;    /* maintain the opnd stack */
		return  0;
	}
    }

    *(long *)lpOpnd[0].values = 0;
    lpOpnd[0].type = LONG_TYPE;
    lpOpnd[0].length = sizeof(long);

    *OpndTop -= ParaNum - 1 ;    /* maintain the opnd stack */
    return  0;

}



/*------------
 !                      _xTStrPut()
 ! tstrget()
 !-----------------------------------------------------------------------*/
short PD_style _xTStrPut( OpndType *lpOpnd, short ParaNum, short *OpndTop )
{
    unsigned char *s;
    char           c;
    int	    n;
    char	    buf1[4096], buf2[4096];

    s = xGetOpndString( &lpOpnd[0] );
    strZcpy(buf1, s, 4096);
    c = xGetOpndChr( &lpOpnd[1] );
    n = xGetOpndLong( &lpOpnd[2] );
    s = xGetOpndString( &lpOpnd[3] );
    strZcpy(buf2, s, 4096);

    if( strNputStr(buf1, 4096, c, n, buf2) == NULL ) {
	*OpndTop -= ParaNum - 1 ;    /* maintain the opnd stack */
	return  1;
    }

    lpOpnd[0].length = strlen(buf1);
    if( lpOpnd[0].length >= MAX_OPND_LENGTH ) {
	s = strdup( s );
	*(long *)(lpOpnd[0].values) = (long)s;
    } else {
	strcpy(lpOpnd[0].values, buf1);
    }

    lpOpnd[0].oval = NULL;
    lpOpnd[0].type = STRING_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} //end of function _xTStrPut()



/*------------
 !                      _xRelSkip()
 ! relskip()
 !-----------------------------------------------------------------------*/
short PD_style _xRelSkip(OpndType *lpOpnd, short ParaNum, short *OpndTop)
{
extern WSToMT FromToStru fFrTo;
    int   i;
    long  l, ll;
    long  offset;
    char  *sz;
    bHEAD *bh;

    i = xGetOpndLong( &lpOpnd[0] ) - 1;
    sz = xGetOpndString( &lpOpnd[1] );
    offset = xGetOpndLong( &lpOpnd[2] );

    if( i <= 0 || i >= fFrTo.cSouDbfNum || sz == NULL ) {
	return ErrorSet.xERROR = iRelSkipParaErr;
    }

    bh = fFrTo.syncBh[i];
    if( bh == NULL ) {
	return ErrorSet.xERROR = iRelSkipParaErr;
    }

    if( *sz != '\0' ) {
	if( IndexSeek(bh, sz) == LONG_MIN ) {
	    l = 0;
	} else {
	    if( offset > 0 )
	    {
		l = IndexStrEqSkip(bh, sz, offset);
	    }
	}
    } else {
	while( --offset ) {
	    ll = IndexEqSkip(bh, 1);
	    if( ll == LONG_MIN )
		break;
	    l = ll;
	}

	//reload the record
	get1rec(fFrTo.cSouFName[i]);
    }

    *(long *)lpOpnd[0].values = l;
    lpOpnd[0].type = LONG_TYPE;

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xRelSkip() */



/*------------
 !                      _xTTotal()
 ! ttotal
 !-----------------------------------------------------------------------*/
short PD_style _xTTotal( OpndType *lpOpnd, short ParaNum, short *OpndTop)
{
    char  *sz;
    dFILE *dfs, *dft;
    int   i, iret;
    char  buf[260];

    sz = xGetOpndString( &lpOpnd[0] );
    szAsqlPath(sz, buf);

    dft = dAwake(buf, DOPENPARA);
    if( dft == NULL ) {
	//ret 1
	iret = 0;
	goto  _xTTotal_EXIT;
    }

    for(i = 1;  i < ParaNum;  i++) {
	sz = xGetOpndString(&lpOpnd[i]);
	szAsqlPath(sz, buf);
	dfs = dAwake(buf, DOPENPARA);
	if( dfs == NULL ) {
	    break;
	}
	dTableSum(dft, dfs);
	dSleep(dfs);
    }
    iret = i;
    dSleep(dft);

_xTTotal_EXIT:
    *(long *)lpOpnd[0].values = iret;
    lpOpnd[0].type = LONG_TYPE;

    *OpndTop -= ParaNum - 1;

    return  0;

} /* end of function _xTTotal() */


/*------------
 !                      _xHighNum()
 ! protocol: xhighnum(ARRAY_TYPE, 5)
 !-----------------------------------------------------------------------*/
// #pragma argsused /* Remed by NiuJingyu */
short PD_style _xHighNum( OpndType *lpOpnd, short ParaNum, short *OpndTop)
{

    ArrayType *p;
    double     f;
    int	       i, j;
    long       l;

    if( lpOpnd[0].type != ARRAY_TYPE ) {
	*OpndTop -= ParaNum;
	return  1;
    }

    p = (ArrayType *)*(long *)lpOpnd[0].oval;
    if( p->DimNum != 0 ) {
	*OpndTop -= ParaNum;
	return  1;
    }

    if( p->ElementNum <= 0 )
    { //if there is no element exist
      //exit directly
      //
      switch( p->ElementType ) {
	case LONG_TYPE:
	   *(long *)lpOpnd[0].values = 0;
	   lpOpnd[0].type = LONG_TYPE;
	   break;

	case INT_TYPE:
	   *(short *)lpOpnd[0].values = 0;
	   lpOpnd[0].type = INT_TYPE;
	   break;

	case CHR_TYPE:
	   *(char *)lpOpnd[0].values = '\0';
	   lpOpnd[0].type = CHR_TYPE;
	   break;

	case FLOAT_TYPE:
	   *(double *)lpOpnd[0].values = 0.0;
	   lpOpnd[0].type = FLOAT_TYPE;
      }

      *OpndTop -= ParaNum - 1;
      return  0;

    } //end of if

    f = xGetOpndFloat( &lpOpnd[1] ) / 100.0;
    i = p->ElementNum - (int)(p->ElementNum * f + 0.5);
    if( i < 0 )
	i = 0;
    if( i >= p->ElementNum )
	i = p->ElementNum - 1;

    switch( p->ElementType ) {
	case LONG_TYPE:
	   qsort(p->ArrayMem, p->ElementNum, sizeof(long), long_sort_function);

	   l = 0;
	   j = p->ElementNum - i;
	   for(  ;  i < p->ElementNum;  i++) {
		l += *((long *)(p->ArrayMem)+i);
	   }

	   *(long *)lpOpnd[0].values = l / j;
	   lpOpnd[0].type = LONG_TYPE;
	   break;

	case INT_TYPE:
	   qsort(p->ArrayMem, p->ElementNum, sizeof(short), short_sort_function);

	   l = 0;
	   j = p->ElementNum - i;
	   for(  ;  i < p->ElementNum;  i++) {
		l += *((short *)(p->ArrayMem)+i);
	   }

	   *(short *)lpOpnd[0].values = l / j;
	   lpOpnd[0].type = INT_TYPE;
	   break;

	case CHR_TYPE:
	   qsort(p->ArrayMem, p->ElementNum, sizeof(char), char_sort_function);

	   l = 0;
	   j = p->ElementNum - i;
	   for(  ;  i < p->ElementNum;  i++) {
		l += *((char *)(p->ArrayMem)+i);
	   }

	   *(char *)lpOpnd[0].values = l / j;
	   lpOpnd[0].type = CHR_TYPE;
	   break;

	case FLOAT_TYPE:
	   qsort(p->ArrayMem, p->ElementNum, sizeof(double), float_sort_function);

	   f = 0.0;
	   j = p->ElementNum - i;
	   for(  ;  i < p->ElementNum;  i++) {
		f += *((double *)(p->ArrayMem)+i);
	   }

	   *(double *)lpOpnd[0].values = f / j;
	   lpOpnd[0].type = FLOAT_TYPE;
	   break;
    }

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xHighNum() */





/*------------
 !                      _xLowNum()
 ! protocol: xlownum(ARRAY_TYPE, 5)
 !-----------------------------------------------------------------------*/
// #pragma argsused /* Remed by NiuJingyu */
short PD_style _xLowNum( OpndType *lpOpnd, short ParaNum, short *OpndTop)
{

    ArrayType *p;
    double     f;
    int	       i, j;
    long       l;

    if( lpOpnd[0].type != ARRAY_TYPE ) {
	*OpndTop -= ParaNum;
	return  1;
    }

    p = (ArrayType *)*(long *)lpOpnd[0].oval;
    if( p->DimNum != 0 ) {
	*OpndTop -= ParaNum;
	return  1;
    }

    if( p->ElementNum <= 0 )
    { //if there is no element exist
      //exit directly
      //
      switch( p->ElementType ) {
	case LONG_TYPE:
	   *(long *)lpOpnd[0].values = 0;
	   lpOpnd[0].type = LONG_TYPE;
	   break;

	case INT_TYPE:
	   *(short *)lpOpnd[0].values = 0;
	   lpOpnd[0].type = INT_TYPE;
	   break;

	case CHR_TYPE:
	   *(char *)lpOpnd[0].values = '\0';
	   lpOpnd[0].type = CHR_TYPE;
	   break;

	case FLOAT_TYPE:
	   *(double *)lpOpnd[0].values = 0.0;
	   lpOpnd[0].type = FLOAT_TYPE;
      }

      *OpndTop -= ParaNum - 1;
      return  0;

    } //end of if

    f = xGetOpndFloat( &lpOpnd[1] ) / 100.0;
    i = (int)(p->ElementNum * f + 0.5) - 1;
    if( i < 0 )
	i = 0;
    if( i >= p->ElementNum )
	i = p->ElementNum - 1;

    switch( p->ElementType ) {
	case LONG_TYPE:
	   qsort(p->ArrayMem, p->ElementNum, sizeof(long), long_sort_function);

	   l = 0;
	   j = i + 1;
	   for(  ;  i >= 0;  i--) {
		l += *((long *)(p->ArrayMem)+i);
	   }

	   *(long *)lpOpnd[0].values = l / j;
	   lpOpnd[0].type = LONG_TYPE;
	   break;

	case INT_TYPE:
	   qsort(p->ArrayMem, p->ElementNum, sizeof(short), short_sort_function);

	   l = 0;
	   j = i + 1;
	   for(  ;  i >= 0;  i--) {
		l += *((short *)(p->ArrayMem)+i);
	   }

	   *(short *)lpOpnd[0].values = l / j;
	   lpOpnd[0].type = INT_TYPE;
	   break;
	case CHR_TYPE:
	   qsort(p->ArrayMem, p->ElementNum, sizeof(char), char_sort_function);

	   l = 0;
	   j = i + 1;
	   for(  ;  i >= 0;  i--) {
		l += *((char *)(p->ArrayMem)+i);
	   }

	   *(char *)lpOpnd[0].values = l / j;
	   lpOpnd[0].type = CHR_TYPE;
	   break;

	case FLOAT_TYPE:
	   qsort(p->ArrayMem, p->ElementNum, sizeof(double), float_sort_function);

	   f = 0.0;
	   j = i + 1;
	   for(  ;  i >= 0;  i--) {
		f += *((double *)(p->ArrayMem)+i);
	   }

	   *(double *)lpOpnd[0].values = f / j;
	   lpOpnd[0].type = FLOAT_TYPE;
	   break;
    }

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xLowNum() */





/*------------
 !                      _xMidNum()
 ! protocol: xmidnum(ARRAY_TYPE)
 !-----------------------------------------------------------------------*/
// #pragma argsused /* Remed by NiuJingyu */
short PD_style _xMidNum( OpndType *lpOpnd, short ParaNum, short *OpndTop)
{

    ArrayType *p;
    double     f;
    long       l;

    if( lpOpnd[0].type != ARRAY_TYPE ) {
	*OpndTop -= ParaNum;
	return  1;
    }

    p = (ArrayType *)*(long *)lpOpnd[0].oval;
    if( p->DimNum != 0 ) {
	*OpndTop -= ParaNum;
	return  1;
    }


    if( p->ElementNum <= 0 )
    { //if there is no element exist
      //exit directly
      //
      switch( p->ElementType ) {
	case LONG_TYPE:
	   *(long *)lpOpnd[0].values = 0;
	   lpOpnd[0].type = LONG_TYPE;
	   break;

	case INT_TYPE:
	   *(short *)lpOpnd[0].values = 0;
	   lpOpnd[0].type = INT_TYPE;
	   break;

	case CHR_TYPE:
	   *(char *)lpOpnd[0].values = '\0';
	   lpOpnd[0].type = CHR_TYPE;
	   break;

	case FLOAT_TYPE:
	   *(double *)lpOpnd[0].values = 0.0;
	   lpOpnd[0].type = FLOAT_TYPE;
      }

      *OpndTop -= ParaNum - 1;
      return  0;

    } //end of if


    if( ParaNum > 1 ) 
    { //2000.9.30 
	
	int ii, ij;
	int j;

	f = xGetOpndFloat( &lpOpnd[1] ) / 100.0;
	ii = (int)(p->ElementNum * f + 0.5) - 1;
	if( ii < 0 )
	    ii = 0;
	if( ii >= p->ElementNum )
	    ii = p->ElementNum - 1;

	if( ParaNum <= 2 ) 
	    ij = p->ElementNum - 1;
	else {
	    f = xGetOpndFloat( &lpOpnd[2] ) / 100.0;
	    ij = (int)(p->ElementNum * f + 0.5) - 1;
	    if( ij < 0 )
		ij = 0;
	    if( ij >= p->ElementNum )
		ij = p->ElementNum - 1;
	}

	j = ij - ii + 1;

	switch( p->ElementType ) {
	case LONG_TYPE:
	   qsort(p->ArrayMem, p->ElementNum, sizeof(long), long_sort_function);

	   l = 0;
	   for(  ;  ii <= ij;  ii++) {
		l += *((long *)(p->ArrayMem)+ii);
	   }

	   *(long *)lpOpnd[0].values = l / j;
	   lpOpnd[0].type = LONG_TYPE;
	   break;

	case INT_TYPE:
	   qsort(p->ArrayMem, p->ElementNum, sizeof(short), short_sort_function);

	   l = 0;
	   for(  ;  ii <= ij;  ii++) {
		l += *((short *)(p->ArrayMem)+ii);
	   }

	   *(short *)lpOpnd[0].values = l / j;
	   lpOpnd[0].type = INT_TYPE;
	   break;

	case CHR_TYPE:
	   qsort(p->ArrayMem, p->ElementNum, sizeof(char), char_sort_function);

	   l = 0;
	   for(  ;  ii <= ij;  ii++) {
		l += *((char *)(p->ArrayMem)+ii);
	   }

	   *(char *)lpOpnd[0].values = l / j;
	   lpOpnd[0].type = CHR_TYPE;
	   break;

	case FLOAT_TYPE:
	   qsort(p->ArrayMem, p->ElementNum, sizeof(double), float_sort_function);

	   f = 0.0;
	   for(  ;  ii <= ij;  ii++) {
		f += *((double *)(p->ArrayMem)+ii);
	   }

	   *(double *)lpOpnd[0].values = f / j;
	   lpOpnd[0].type = FLOAT_TYPE;
	   break;
	}

    } //end of if( ParaNum > 1 ) 
    
    //
    else
    //continue old method

    switch( p->ElementType ) {
	case LONG_TYPE:
	   qsort(p->ArrayMem, p->ElementNum, sizeof(long), long_sort_function);

	   l = *((long *)(p->ArrayMem) + p->ElementNum / 2);
	   if( (p->ElementNum % 2) == 0 )
		l = ( l + *((long *)(p->ArrayMem) + p->ElementNum / 2 - 1)) / 2;
	   *(long *)lpOpnd[0].values = l;

	   lpOpnd[0].type = LONG_TYPE;
	   break;

	case INT_TYPE:
	   qsort(p->ArrayMem, p->ElementNum, sizeof(short), short_sort_function);

	   l = *((short *)(p->ArrayMem) + p->ElementNum / 2);
	   if( (p->ElementNum % 2) == 0 )
		l = ( l + *((short *)(p->ArrayMem) + p->ElementNum / 2 - 1)) / 2;
	   *(long *)lpOpnd[0].values = l;

	   lpOpnd[0].type = INT_TYPE;
	   break;

	case CHR_TYPE:
	   qsort(p->ArrayMem, p->ElementNum, sizeof(char), char_sort_function);

	   l = *((char *)(p->ArrayMem) + p->ElementNum / 2);
	   if( (p->ElementNum % 2) == 0 )
		l = ( l + *((char *)(p->ArrayMem) + p->ElementNum / 2 - 1)) / 2;
	   *(long *)lpOpnd[0].values = l;

	   lpOpnd[0].type = CHR_TYPE;
	   break;

	case FLOAT_TYPE:
	   qsort(p->ArrayMem, p->ElementNum, sizeof(double), float_sort_function);

	   f = *((double *)(p->ArrayMem) + p->ElementNum / 2);
	   if( (p->ElementNum % 2) == 0 )
		f = ( f + *((double *)(p->ArrayMem) + p->ElementNum / 2 - 1)) / 2.0;

	   *(double *)lpOpnd[0].values = f;

	   lpOpnd[0].type = FLOAT_TYPE;
	   break;
    }

    *OpndTop -= ParaNum - 1;
    return  0;

} /* end of function _xMidNum() */




////////////////////////////////////////////////////////////////////////////
/////// inside function ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

int char_sort_function( const void *a, const void *b)
{
    return  (int)*(char *)a - (int)*(char *)b;
}


int short_sort_function( const void *a, const void *b)
{
    return  (int)*(short *)a - (int)*(short *)b;
}


int long_sort_function( const void *a, const void *b)
{
    if( *(long *)a > *(long *)b )
	return  1;
    if( *(long *)a < *(long *)b )
	return  -1;

    return  0;
}

int float_sort_function( const void *a, const void *b)
{
    if( *(double *)a > *(double *)b )
	return  1;
    if( *(double *)a < *(double *)b )
	return  -1;

    return  0;
}





/************************ end of file XEXPFUN.C *****************************/