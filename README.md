# Laboratorio 10 - EDA

## Integrantes
* Jorge Castillo
* Julisa Lapa
* Luis Carbajal

## Compilar
Tener OpenCV instalado ([  __Guía__  ](https://stackoverflow.com/questions/15320267/package-opencv-was-not-found-in-the-pkg-config-search-path))
Para correr el código, se debe tener seteado el file 'opencv.pc' in "/usr/local/lib/pkgconfig".
Además agregar las siguientes líneas al '.bashrc' o '.zshrc'.

```
PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
export PKG_CONFIG_PATH
```

Para recargar el '.bashrc' o '.zshrc'
```
source .zshrc 
```

Content of 'opencv.pc':
```
prefix=/usr
exec_prefix=${prefix}
includedir=${prefix}/include
libdir=${exec_prefix}/lib

Name: opencv
Description: The opencv library
Version: 2.x.x
Cflags: -I${includedir}/opencv -I${includedir}/opencv2
Libs: -L${libdir} -lopencv_calib3d -lopencv_imgproc -lopencv_core -lopencv_ml -lopencv_features2d -lopencv_objdetect -lopencv_flann -lopencv_video -lopencv_highgui
```

## Uso
Una vez ejecutado el programa se abrirá una ventana con una cuadrícula 8x8. La ventana responde a 6 teclas:
* a: Reduce las dimensiones de la cuadrícula
* s: Aumenta las dimensiones de la cuadrícula
* z: Ejecuta el Z-order sobre la cuadrícula actual
* x: Ejecuta el gray-order sobre la cuadrícula actual
* c: Ejecuta el double gray-order sobre la cuadrícula actual
* q: Cierra la ventana y finaliza el programa