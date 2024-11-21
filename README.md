1- copy the content of the following folder
```
 ~/STM32Cube/Repository/Packs/ARM/mbedTLS/3.1.1/library/ 
 ```
 to the following folder
```
./aware_demo\Middlewares\Third_Party\ARM_Security\library
```

2- in STM21CubeIDE project Exlude from build 

*Middlewares->Third_Party->lwIP_Network_lwIP->ports->cmsis-driver->netif*

3- Project options Preprocessor add

```
MBEDTLS_CONFIG_FILE="config_mbedtls.h"
NO_STSAFE
```

4- Project options include path add

```
${workspace_loc:/${ProjName}/Middlewares/Third_Party/ARM_Security/library}
${workspace_loc:/${ProjName}/Core/Src/mbedTLS}
```