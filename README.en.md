# RXSkin
&emsp;&emsp;RXSkin is an interface library for the Windows platform, which includes a complete set of desktop software development components developed using the C++language. The underlying principle of RXSkin is based on DirectUI technology, which refers to the implementation principle of the duilib library, but does not reference the duilib code at all. It mainly draws on its design ideas. You can directly use the RXSkin interface library to develop commonly used desktop applications, simplifying the UI development work of the application.

### advantage
&emsp;&emsp;1. Based on direnctUI technology, it has a high degree of customizability in UI design   
&emsp;&emsp;2. No dependency on VC runtime library   
&emsp;&emsp;3. You can fully control the interface layout through XML   
&emsp;&emsp;4. Highly scalable, you can refer to certain controls implemented in this library to customize your own unique functional requirements UI controls  
&emsp;&emsp;5. Completely open source, all functions involved can be found in the source code of this library   
&emsp;&emsp;6. Stable and reliable, this library was spun off from the "Ruixiang Remote Desktop" product, and the UI implementation of this product is completely based on this library without any cropping. We promise that   any related optimizations and modifications made by "Ruixiang Remote Desktop" will be synchronized to this library in a timely manner.   

### Compilation instructions
&emsp;&emsp;1> Hardware requirements: CPU: 2.2GHz or above, memory: 8GB or above, hard drive: 100GB or above   
&emsp;&emsp;2> System requirements: Win10/11, vs 2019 and above versions   

### Compilation process
&emsp;&emsp;1> In the tools sub directory of the project root directory, execute buid.exe, follow the prompts to select the visual studio version, and then generate the compilation script: build_stln.bat,
&emsp;&emsp;2> Execute: build_stln.bat, quietly wait for compilation to complete
&emsp;&emsp;3> After the compilation is completed, the lib file will be found in the lib directory under the root directory, and the dll file will be found in the bin directory
&emsp;&emsp;4> Wishing you success and happiness

### Effect image
<img src="http://www.rxiang.net/img/6.png">
