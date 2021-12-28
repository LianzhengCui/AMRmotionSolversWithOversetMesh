# AMRmotionSolversWithOversetMesh
## description

Adaptive mesh refinement for mesh motion solvers including overset mesh. 

**Authours:**

Lianzheng Cui lzhcui@sjtu.edu.cn

Zuogang Chen  zgchen@sjtu.edu.cn


**The library is based on:**

OpenFOAM-v2106, www.openfoam.com.

Henning Scheufler, https://github.com/HenningScheufler/multiDimAMR.

Rettenmaier, Daniel, et al. "Load balanced 2D and 3D adaptive mesh refinement in OpenFOAM." SoftwareX 10 (2019): 100317. link: https://www.sciencedirect.com/science/article/pii/S2352711018301699

## Prerequisites
install OpenFOAM-v2106(better) or OpenFOAM-v2012

```https://www.openfoam.com/download/release-history```
## Installing
```git clone https://github.com/LianzhengCui/AMRmotionSolversWithOversetMesh.git```

```cd AMRmotionSolversWithOversetMesh```

```./Allwmake```

Please note it is normal that some warnings may appear when compiling the "AMRmotionSolvers".

## Usage
Two tutorial cases were offered. Please refer to the files for usage.

The AMRfloatingObject includes mesh motion and adaptive mesh refinement.

The oversetAMRfloatingObject includes overset mesh motion and mesh refinement.

Please note that the load-balancing is unavailable temporarily in this version.

To run the turorial cases

```./Allrun```

To clear the solutions

```./Allclean```

If you got a Permission denied when running a file, type

```chmod +x <name of the file>```

## License
This project is licensed under the GPL v3 License.

http://www.gnu.org/licenses
