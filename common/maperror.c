/* maperror.c - Error mapping
 *	Copyright (C) 2001, 2002 Free Software Foundation, Inc.
 *
 * This file is part of GnuPG.
 *
 * GnuPG is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * GnuPG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <config.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <ksba.h>
#include <assuan.h>

#include "util.h"
#include "errors.h"

/* Note: we might want to wrap this in a macro to get our hands on
   the line and file where the error occured */
int
map_ksba_err (int err)
{
  switch (err)
    {
    case -1:
    case 0: 
      break;

    case KSBA_Out_Of_Core: err = GNUPG_Out_Of_Core; break;
    case KSBA_Invalid_Value: err = GNUPG_Invalid_Value; break;
    case KSBA_Not_Implemented: err = GNUPG_Not_Implemented; break;
    case KSBA_Conflict: err = GNUPG_Conflict; break;
    case KSBA_Read_Error: err = GNUPG_Read_Error; break;
    case KSBA_Write_Error: err = GNUPG_Write_Error; break;
    case KSBA_No_Data: err = GNUPG_No_Data; break;
    case KSBA_Bug: err = GNUPG_Bug; break;
    case KSBA_Unsupported_Algorithm: err = GNUPG_Unsupported_Algorithm; break;
    case KSBA_Invalid_Index: err = GNUPG_Invalid_Index; break;
    case KSBA_Invalid_Sexp: err = GNUPG_Invalid_Sexp; break;
    case KSBA_Unknown_Sexp: err = GNUPG_Unknown_Sexp; break;
      
    default:
      err = seterr (General_Error);
      break;
    }
  return err;
}


int 
map_gcry_err (int err)
{
  switch (err)
    {
    case GCRYERR_EOF:
    case -1:
      err = -1;
      break;

    case 0:
      break;

    case GCRYERR_WRONG_PK_ALGO:
    case GCRYERR_INV_PK_ALGO:
    case GCRYERR_INV_MD_ALGO:
    case GCRYERR_INV_CIPHER_ALGO:
      err = GNUPG_Unsupported_Algorithm;
      break;
    case GCRYERR_INV_KEYLEN:
    case GCRYERR_WEAK_KEY:
    case GCRYERR_BAD_PUBLIC_KEY: err = GNUPG_Bad_Public_Key; break;
    case GCRYERR_BAD_SECRET_KEY: err = GNUPG_Bad_Secret_Key; break;
    case GCRYERR_BAD_SIGNATURE:  err = GNUPG_Bad_Signature; break;

    case GCRYERR_BAD_MPI:
      err = GNUPG_Bad_Data;
      break;
      
    case GCRYERR_INV_ARG:
    case GCRYERR_INV_OP:
    case GCRYERR_INTERNAL:
    case GCRYERR_INV_CIPHER_MODE:
      err = GNUPG_Invalid_Value;
      break;

    case GCRYERR_SELFTEST: 
      err = GNUPG_Bug;
      break;

    case GCRYERR_SEXP_INV_LEN_SPEC    :
    case GCRYERR_SEXP_STRING_TOO_LONG :
    case GCRYERR_SEXP_UNMATCHED_PAREN :
    case GCRYERR_SEXP_NOT_CANONICAL   :
    case GCRYERR_SEXP_BAD_CHARACTER   :
    case GCRYERR_SEXP_BAD_QUOTATION   :
    case GCRYERR_SEXP_ZERO_PREFIX     :
    case GCRYERR_SEXP_NESTED_DH       :
    case GCRYERR_SEXP_UNMATCHED_DH    :
    case GCRYERR_SEXP_UNEXPECTED_PUNC :
    case GCRYERR_SEXP_BAD_HEX_CHAR    :
    case GCRYERR_SEXP_ODD_HEX_NUMBERS :
    case GCRYERR_SEXP_BAD_OCT_CHAR    :
      err = GNUPG_Invalid_Sexp;
      break;

    case GCRYERR_NO_MEM: err = GNUPG_Out_Of_Core; break;

    case GCRYERR_NOT_IMPL:  err = GNUPG_Not_Implemented; break;
    case GCRYERR_CONFLICT:  err = GNUPG_Conflict; break;
      
    case GCRYERR_INV_OBJ:   /* an object is not valid */
    case GCRYERR_TOO_SHORT: /* provided buffer too short */
    case GCRYERR_TOO_LARGE: /* object is too large */
    case GCRYERR_NO_OBJ:    /* Missing item in an object */
    default:
      err = seterr (General_Error);
      break;
    }
  return err;
}

int 
map_kbx_err (int err)
{
  switch (err)
    {
    case -1:
    case 0:
      break;
      
    default:
      err = seterr (General_Error);
      break;
    }
  return err;
}


int 
map_assuan_err (int err)
{
  switch (err)
    {
    case -1:
    case 0:
      break;

    case ASSUAN_Canceled:        err = GNUPG_Canceled; break;
    case ASSUAN_Invalid_Index:   err = GNUPG_Invalid_Index; break;

    case ASSUAN_Not_Implemented: err = GNUPG_Not_Implemented; break;
    case ASSUAN_Server_Fault:    err = GNUPG_Assuan_Server_Fault; break;
    case ASSUAN_No_Public_Key:   err = GNUPG_No_Public_Key; break;
    case ASSUAN_No_Secret_Key:   err = GNUPG_No_Secret_Key; break;

    case ASSUAN_Cert_Revoked:    err = GNUPG_Certificate_Revoked; break;
    case ASSUAN_No_CRL_For_Cert: err = GNUPG_No_CRL_Known; break;       
    case ASSUAN_CRL_Too_Old:     err = GNUPG_CRL_Too_Old; break;        

    case ASSUAN_Not_Trusted:     err = GNUPG_Not_Trusted; break;

    case ASSUAN_Card_Error:      err = GNUPG_Card_Error; break;
    case ASSUAN_Invalid_Card:    err = GNUPG_Invalid_Card; break;
    case ASSUAN_No_PKCS15_App:   err = GNUPG_No_PKCS15_App; break;
    case ASSUAN_Card_Not_Present: err= GNUPG_Card_Not_Present; break;
    case ASSUAN_Not_Confirmed:   err = GNUPG_Not_Confirmed; break;
    case ASSUAN_Invalid_Id:      err = GNUPG_Invalid_Id; break;

    default:
      err = err < 100? GNUPG_Assuan_Server_Fault : GNUPG_Assuan_Error;
      break;
    }
  return err;
}

/* Map GNUPG_xxx error codes to Assuan status codes */
int
map_to_assuan_status (int rc)
{
  switch (rc)
    {
    case -1:
      rc = ASSUAN_No_Data_Available;
      break;
    case 0: break;
    case GNUPG_Bad_CA_Certificate: 
    case GNUPG_Bad_Certificate:  
    case GNUPG_Wrong_Key_Usage:
    case GNUPG_Certificate_Revoked:
    case GNUPG_No_CRL_Known:
    case GNUPG_CRL_Too_Old:
    case GNUPG_No_Policy_Match:
    case GNUPG_Certificate_Expired:
      rc = ASSUAN_Bad_Certificate; 
      break;
    case GNUPG_Bad_Certificate_Chain: rc = ASSUAN_Bad_Certificate_Chain; break;
    case GNUPG_Missing_Certificate: rc = ASSUAN_Missing_Certificate; break;
    case GNUPG_No_Data:           rc = ASSUAN_No_Data_Available; break;
    case GNUPG_Bad_Signature:     rc = ASSUAN_Bad_Signature; break;
    case GNUPG_Not_Implemented:   rc = ASSUAN_Not_Implemented; break;
    case GNUPG_No_Agent:          rc = ASSUAN_No_Agent; break;
    case GNUPG_Agent_Error:       rc = ASSUAN_Agent_Error; break;
    case GNUPG_No_Public_Key:     rc = ASSUAN_No_Public_Key; break;
    case GNUPG_No_Secret_Key:     rc = ASSUAN_No_Secret_Key; break;
    case GNUPG_Invalid_Data:      rc = ASSUAN_Invalid_Data; break;
    case GNUPG_Invalid_Name:      rc = ASSUAN_Invalid_Name; break;
    case GNUPG_Not_Trusted:       rc = ASSUAN_Not_Trusted; break;
    case GNUPG_Canceled:          rc = ASSUAN_Canceled; break;
    case GNUPG_Invalid_Index:     rc = ASSUAN_Invalid_Index; break;

    case GNUPG_Card_Error:      
    case GNUPG_Card_Reset:      
      rc = ASSUAN_Card_Error; 
      break;
    case GNUPG_Card_Removed:  
    case GNUPG_Card_Not_Present:
      rc = ASSUAN_Card_Not_Present;
      break;
    case GNUPG_Invalid_Card:      rc = ASSUAN_Invalid_Card; break;
    case GNUPG_No_PKCS15_App:     rc = ASSUAN_No_PKCS15_App; break;
    case GNUPG_Not_Confirmed:     rc = ASSUAN_Not_Confirmed; break;
    case GNUPG_Invalid_Id:        rc = ASSUAN_Invalid_Id; break; 

    case GNUPG_Bad_PIN:
    case GNUPG_Bad_Passphrase:
      rc = ASSUAN_No_Secret_Key;
      break;

    case GNUPG_Read_Error: 
    case GNUPG_Write_Error:
    case GNUPG_IO_Error: 
      rc = ASSUAN_Server_IO_Error;
      break;
    case GNUPG_Out_Of_Core:    
    case GNUPG_Resource_Limit: 
      rc = ASSUAN_Server_Resource_Problem;
      break;
    case GNUPG_Bug: 
    case GNUPG_Internal_Error:   
      rc = ASSUAN_Server_Bug;
      break;
    default: 
      rc = ASSUAN_Server_Fault;
      break;
    }
  return rc;
}







