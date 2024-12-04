#ifndef _CERTS_H_
#define _CERTS_H_

/**************************** AWS Root CA  ************************/
extern const char AmazonRootCA3[];
extern const uint32_t AmazonRootCA3_size;

#if defined(NO_STSAFE)
/**************************** Device cert  ************************/
extern const char device_cert[];
extern const uint32_t device_cert_size;

/**************************** Device PVK  *************************/
extern const char device_pvk[];
extern const uint32_t device_pvk_size;

/**************************** Device PBK  *************************/
extern const char device_pbk[];
extern const uint32_t device_pbk_size;

#endif /* NO_STSAFE */
#endif /* _CERTS_H_ */
