/* wolfSSL naming convention */

#define QSC_SIGS \
    oid_add_from_string("falcon_level1", "1.3.9999.3.1"); \
    oid_add_from_string("falcon_level5", "1.3.9999.3.4"); \
    oid_add_from_string("dilithium_level2", "1.3.6.1.4.1.2.267.7.4.4"); \
    oid_add_from_string("dilithium_level3", "1.3.6.1.4.1.2.267.7.6.5"); \
    oid_add_from_string("dilithium_level5", "1.3.6.1.4.1.2.267.7.8.7"); \
    oid_add_from_string("dilithium_aes_level2", "1.3.6.1.4.1.2.267.11.4.4"); \
    oid_add_from_string("dilithium_aes_level3", "1.3.6.1.4.1.2.267.11.6.5"); \
    oid_add_from_string("dilithium_aes_level5", "1.3.6.1.4.1.2.267.11.8.7");

#define QSC_KEMS \
    { 532, "ntru_hps_level1" }, \
    { 533, "ntru_hps_level3" }, \
    { 534, "ntru_hps_level5" }, \
    { 535, "ntru_hrss_level3" }, \
    { 536, "saber_level1" }, \
    { 537, "saber_level3" }, \
    { 538, "saber_level5" }, \
    { 570, "kyber_level1" }, \
    { 572, "kyber_level3" }, \
    { 573, "kyber_level5" }, \
    { 574, "kyber_90s_level1" }, \
    { 575, "kyber_90s_level3" }, \
    { 576, "kyber_90s_level5" }, \
    { 12052, "p256_ntru_hps_level1" }, \
    { 12053, "p384_ntru_hps_level3" }, \
    { 12054, "p521_ntru_hps_level5" }, \
    { 12055, "p384_ntru_hrss_level3" }, \
    { 12056, "p256_saber_level1" }, \
    { 12057, "p384_saber_level3" }, \
    { 12058, "p521_saber_level5" }, \
    { 12090, "p256_kyber_level1" }, \
    { 12092, "p384_kyber_level3" }, \
    { 12093, "p521_kyber_level5" }, \
    { 12094, "p256_kyber_90s_level1" }, \
    { 12095, "p384_kyber_90s_level3" }, \
    { 12096, "p521_kyber_90s_level5" }, \
    { 0xfe0b, "falcon_level1" }, \
    { 0xfe0e, "falcon_level5" }, \
    { 0xfea0, "dilithium_level2" }, \
    { 0xfea3, "dilithium_level3" }, \
    { 0xfea5, "dilithium_level5" }, \
    { 0xfea7, "dilithium_aes_level2" }, \
    { 0xfeaa, "dilithium_aes_level3" }, \
    { 0xfeac, "dilithium_aes_level5" },

