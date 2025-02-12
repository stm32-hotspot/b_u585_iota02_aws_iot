
/* Includes --------------------------------------------------------------------*/

#include "stsafea_mbedtls_interface.h"
#if defined(__USE_STSAFE__)
/* Functions -------------------------------------------------------------------*/

/* Set for STSAFE Zone 0 */
#if defined (MBEDTLS_ECDSA_SIGN_ALT)
int mbedtls_ecdsa_sign( mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s,
                        const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
                        int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
  int32_t 			      ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA; /* mbedtls status code */

  StSafeA_HashTypes_t digest_type;
  uint8_t 			      RSLength = 0;

  StSafeA_LVBuffer_t  SignR;
  uint8_t 			      data_SignR[ STSAFEA_XYRS_ECDSA_SHA256_LENGTH ] = { 0 };
                      SignR.Length = 0;
                      SignR.Data = data_SignR;

  StSafeA_LVBuffer_t  SignS;
  uint8_t 			      data_SignS[ STSAFEA_XYRS_ECDSA_SHA256_LENGTH ] = { 0 };
                      SignS.Length = 0;
                      SignS.Data = data_SignS;


  /* Check ec algo type from public key is compatible with Slot 0 key algo */
  if ( grp->id != MBEDTLS_ECP_DP_SECP256R1 )
    return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

  /* Check the hash algorithm size and hash length */
  if ( blen >= STSAFEA_XYRS_ECDSA_SHA256_LENGTH )
  {
    SignR.Length = STSAFEA_XYRS_ECDSA_SHA256_LENGTH;
    SignS.Length = STSAFEA_XYRS_ECDSA_SHA256_LENGTH;
    digest_type = STSAFEA_SHA_256;
    RSLength = STSAFEA_XYRS_ECDSA_SHA256_LENGTH;
  }
	else
	{
		return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
	}

	/* STSAFE-A ECDSA signature generation function call */
	if ( SAFEA1_ECDSA_Sign( 0, // Slot 0
	                        buf,
	                        digest_type,
	                        RSLength,
	                        &SignR,
	                        &SignS ) != STSAFEA_OK )
	{
	  return ( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
	}

	/* Format signature result to match mbed's original code, i.e ecdsa_signature_to_asn1() */
	ret = mbedtls_mpi_read_binary( r, SignR.Data, RSLength );
	if ( ret == 0 ) ret = mbedtls_mpi_read_binary( s, SignS.Data, RSLength );

	return( ret );
}
#endif


#if defined(MBEDTLS_ECDSA_SIGN_ALT) || defined(MBEDTLS_ECDSA_VERIFY_ALT)
int mbedtls_ecdsa_can_do( mbedtls_ecp_group_id gid )
{
  switch( gid )
  {
    case MBEDTLS_ECP_DP_SECP256R1: return 1;
    default:                       return 0;
  }
}
#endif
#endif /* #if defined(__USE_STSAFE__) */
