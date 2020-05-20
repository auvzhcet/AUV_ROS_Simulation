# Welcome to the AUV-ROS-Simulation wiki!
*To contribute or edit, please clone it to your local repository using
`git clone https://github.com/auvzhcet/AUV_ROS_Simulation.wiki.git`*

---
## Introduction
This repository gives the insights for designing 3-D models for arenas and controlling of AUV in simulations.

---
## Content
1. [To open the simulation](https://github.com/auvzhcet/AUV_ROS_Simulation/wiki#to-open-the-simulation)
2. [To import these models in Gazebo GUI](https://github.com/auvzhcet/AUV_ROS_Simulation/wiki#to-import-these-models-in-gazebo-gui)
3. [Softwares used](https://github.com/auvzhcet/AUV_ROS_Simulation/wiki#softwares-used)
4. [Debugging](https://github.com/auvzhcet/AUV_ROS_Simulation/wiki#debugging)
5. [Resources/Tutorials](https://github.com/auvzhcet/AUV_ROS_Simulation/wiki#resourcestutorials)

---
### To open the simulation
1. Copy all the model folders present in
  a. SAVe folder
  ```bash
  sea_simulation/models_sea/SAVe_models
  ```
  b. SAUVC folder
  ```
  sea_simulation/models_sea/SAUVC_models
  ```
into your Gazebo directory
```
~/.gazebo/models/
 ```
 

2. Navigate to 
 ```bash
 cd /home/AUV_SOFTWARE/sea_simulation/
 ```
 and run
 ```
 gazebo sea_simul.world
 ```

---
### To import these models in Gazebo GUI
1. Repeat step 1 of [To open the simulation](https://github.com/auvzhcet/AUV_ROS_Simulation/wiki#to-open-the-simulation)
2. Launch gazebo using `gazebo`
3. Drag and drop the props from the `insert` dropdown menu.

---
### To build your own props and import it in Gazebo
 
To import your own model in gazebo follow this tutorial: 
[Download a 3D model and load it into a Gazebo simulation](https://www.youtube.com/watch?v=aP4sDyrRzpU&t=561s)
*If the objects are static you can keep the values of `<inertia>` tag `0 0 0`*

---
### Softwares used

1. [Google Sketchup Pro 2019](https://www.sketchup.com/products/sketchup-pro)
2. [Blender](https://www.blender.org/download/)
3. [SolidWorks](https://www.solidworks.com/sw/support/downloads.htm)

---
### Debugging
1. If your model falls, whenever you spawn it in gazebo, readjust the `center of mass` of the prop in Blender.

---
### Resources/Tutorials

[Download a 3D model and load it into a Gazebo simulation](https://www.youtube.com/watch?v=aP4sDyrRzpU&t=561s)

[[ROS Tutorials] How to open a solidworks model in gazebo](https://www.youtube.com/watch?v=T7X_p_KMwus)

[[ROS Q&A] 054 - buoyancy neutral object goes up with hydrodynamics plugin #Part 1](https://www.youtube.com/watch?v=NgmvhSEM5SQ)

[[ROS Q&A] 066 - buoyancy neutral object goes up with hydrodynamics plugin #Part 2](https://www.youtube.com/watch?v=evweU2MjNjc)

[[ROS Tutorials] 068 - buoyancy neutral object goes up with hydrodynamics plugin #Part 3](https://www.youtube.com/watch?v=J6N96Tpid7A)
