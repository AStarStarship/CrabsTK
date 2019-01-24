# seams.0_3_4.igeek

## 1 Overview

<a href="http://2.bp.blogspot.com/--Iak2F2MpPE/U38Yahs0UyI/AAAAAAAACDM/i6iG1TPTZ4g/s1600/Robot+L-V+Logo.1.png" imageanchor="1"><img src="http://2.bp.blogspot.com/--Iak2F2MpPE/U38Yahs0UyI/AAAAAAAACDM/i6iG1TPTZ4g/s1600/Robot+L-V+Logo.1.png" /></a>

The Interactive Genetic Evolutionary Environment for Kabuki Toolkit is an application of biomimicry in computer science. It is abstract simulation of the human body immune system on the cellular level with quasi-robotic white blood cells that fight off genetically modified retroviruses. The goal of the simulation is for either the viruses to kill the host, the host to kill the viruses, or for the two to maintain a symbiotic relationship.

This project is ongoing research in computer science and engineering and is currently in its early stages. The goal of the experiment is to innovate on classical genetic algorithm and programming techniques by mimicking the way that the human body rights off infections and cancerous cells. Traits identified as undesirable (i.e. viruses and cancer) will illicit an Autoimmune response that will force the undesired trait to evolve or die, potentially allowing for a more sophisticated fitness function generation.

Currently there isn't a whole lot to see at the moment. There is a non-working 2D genetic shape matching algorithm that collapses in on itself and a non-working low-quality software rendered 3D graphics engine. The goal is then to move from 2D into 3D, from 3D into 4D, and then into solving complex optimizations. The end-goal for IGEEK Toolkit is to provide an AIML GUI generator to A-B test UX designs using design thinking.

### 1.1 Content Table

1.  [Overview](#1-overview)
    1. [Content Table](#11-content-table)
	2. [Components](#12-components)
2.  [Biomimicry](#2-biomimicry)

### 1.2 Components

There are four primary entities in the simulation.

* [[Host]]
* [[Cells]]
* [[Genetically Engineered Retroviruses]]
* [[Robotic Leukocytes]]

In order to understand the way the simulation works you need to know some relevant background information about how the human body immune system and viruses work. Please watch the videos bellow so you will have a better understanding of what the simulation behaves the way it does.

<a href="http://www.youtube.com/watch?feature=player_embedded&v=Rpj0emEGShQ
" target="_blank"><img src="http://img.youtube.com/vi/Rpj0emEGShQ/0.jpg" 
alt="YouTube video on how the immune system work." width="560" height="315" border="10" /></a>

![Early alpha screenshot.](http://2.bp.blogspot.com/-Mw2orosoLzA/U7Gq4H9nbXI/AAAAAAAACEA/3Z2eJgz7-9M/s1600/alpha_screenshot_1.small.png)

## 2 Biomimicry

### 2.1 Immune System

This is a short video series that goes more in depth over how the human body immune system works.

<a href="http://www.youtube.com/watch?feature=player_embedded&v=HUa_I7ZjSYs
" target="_blank"><img src="http://img.youtube.com/vi/HUa_I7ZjSYs/0.jpg" 
alt="Blood and the Immune System Overview" width="560" height="315" border="10" /></a>

#### Cell Components
<img src="http://employees.csbsju.edu/hjakubowski/classes/ch250/CellImageMapB.jpg" />

#### T Cell Receptors
The first thing to understand for this simulation is how viruses are brought into the cell. You can read the Wikipedia page on <a href="http://en.wikipedia.org/wiki/T-cell_receptor" target="_blank">T cell Receptors</a> for more information.

<a href="http://www.youtube.com/watch?feature=player_embedded&v=kIxmiTuRydw" target="_blank"><img src="http://img.youtube.com/vi/kIxmiTuRydw/0.jpg" 
alt="YouTube video about how T cells work." width="560" height="315" border="10" /></a>

#### Stem Cells

<a href="http://www.youtube.com/watch?feature=player_embedded&v=ntk8XsxVDi0" target="_blank"><img src="http://img.youtube.com/vi/ntk8XsxVDi0/0.jpg" 
alt="YouTube video about how stem cells work." width="560" height="315" border="10" /></a>

#### White Blood Cells

<a href="http://www.youtube.com/watch?feature=player_embedded&v=RO6qmpApyDM
" target="_blank"><img src="http://img.youtube.com/vi/RO6qmpApyDM/0.jpg" 
alt="YouTube video about how T cells work." width="560" height="315" border="10" /></a>

##### Leukocytes Classes

<table>
<tr><td><b>Type</b> </td><td><b>Aprox. %</b></td><td><b>Diameter (um)</td><td><b>Targets</b></td></tr>
<tr><td>Neutrophil</td><td>62</td><td>10–12</td><td>Bacteria, Fungi</td></tr>
<tr><td>Eosinophil</td><td>2.3</td><td>10–12</td><td>Larger parasites, Modulate allergic inflammatory responses</td></tr>
<tr><td>Macrophage</td><td>21</td><td>Ingests foreign particles and dead cells.</td></tr>
<tr><td>Basophil</td><td>0.4</td><td>12–15</td><td>Release histamine for inflammatory responses</td></tr>
<tr><td>Lymphocyte</td><td>30</td><td>Small:7–8 Large:12–15</td><td>B Cells, T cells, and Natural Killer Cells</td></tr>
<tr><td>Monocyte</td><td>5.3</td><td>12–20</td><td>Disburse into tissue in body.</td></tr>
</table>

###### Neutrophil

<a href="http://www.youtube.com/watch?feature=player_embedded&v=0TvTyj5FAaQ
" target="_blank"><img src="http://img.youtube.com/vi/0TvTyj5FAaQ/0.jpg" 
alt="YouTube video about how T cells work." width="560" height="315" border="10" /></a>

###### T & B Cells

<ul>
<li>T cells and B cells
<ul>
<li>B Cells - Release antibodies and assist the activation of T cells.</li>
<li>T Cells - </li>
</ul>
<li>Natural killer cells - Seek out and kill tumors and virally infected cells</li>
</ul>


<a href="http://www.youtube.com/watch?feature=player_embedded&v=HNP1EAYLhOs
" target="_blank"><img src="http://img.youtube.com/vi/HNP1EAYLhOs/0.jpg" 
alt="YouTube video about how Natural Killer Cell work in the human immune system." width="560" height="315" border="10" /></a>

<a href="http://www.youtube.com/watch?feature=player_embedded&v=Rpj0emEGShQ
" target="_blank"><img src="http://img.youtube.com/vi/Rpj0emEGShQ/0.jpg" 
alt="YouTube video on how the immune system work." width="560" height="315" border="10" /></a>

### 2.2 Cells

### Cell Components

<img src="http://employees.csbsju.edu/hjakubowski/classes/ch250/CellImageMapB.jpg" />

#### T Cell Receptors

The first thing to understand for this simulation is how viruses are brought into the cell. You can read the Wikipedia page on <a href="http://en.wikipedia.org/wiki/T-cell_receptor" target="_blank">T cell Receptors</a> for more information.

<a href="http://www.youtube.com/watch?feature=player_embedded&v=RO6qmpApyDM
" target="_blank"><img src="http://img.youtube.com/vi/RO6qmpApyDM/0.jpg" 
alt="YouTube video about how T cells work." width="560" height="315" border="10" /></a>

#### Stem Cells

<a href="http://www.youtube.com/watch?feature=player_embedded&v=jF2iXpoG5j8
" target="_blank"><img src="http://img.youtube.com/vi/jF2iXpoG5j8/0.jpg" 
alt="YouTube video about how stem cells work." width="560" height="315" border="10" /></a>

### 2.3 Viruses

#### 2.3.a Virus Shape

Most viruses that effect animals have <a href="http://en.wikipedia.org/wiki/Icosahedron">Icosahedral</a> shell structures. An icosohedron is a polygon with 12 vertexes and 20 faces.

![From Wikipedia: Icosahedron vertexes form three orthogonal golden rectangles](http://upload.wikimedia.org/wikipedia/commons/9/9c/Icosahedron-golden-rectangles.svg)

#### 2.3.b Viral Replication

<a href="http://www.youtube.com/watch?feature=player_embedded&v=PHp6iYDi9ko
" target="_blank"><img src="http://img.youtube.com/vi/PHp6iYDi9ko/0.jpg" 
alt="Introduction to viral replication on YouTube." width="560" height="315" border="10" /></a>

##### Replication Methods

There are three distinct types of virus replication methods based on how they  infect cells.
<table>
<tr>
<td>Mechanism</td>
<td>Distinction</td>
<td>Example</td>
</tr>
<tr>
<td><a href="http://en.wikipedia.org/wiki/DNA_virus">DNA Viruses</a></td>
<td>Replication Occurs inside of the nucleolus.</td>
<td>Herpes Viruses, smallpox, chickenpox</td>
</tr>
<tr>
<td><a href="http://en.wikipedia.org/wiki/RNA_virus">RNA viruses</a></td>
<td>RNA Replication occurs inside of the cytoplasm. </td>
<td>SARS, influenza, hepatitis C, West Nile fever, polio and measles.</td>
</tr>
<td><a href="http://en.wikipedia.org/wiki/Retrovirus">Retrovirus</a></td>
<td><a href="http://en.wikipedia.org/wiki/Obligate_parasite">Obligate parasitic</a> RNA virus that takes over the cell.</td>
<td>HIV</td>
</tr>
</table>

RNA Viruses can make 500 viral copies of themselves after infecting a cell.

##### HIV Virus

<a href="http://www.youtube.com/watch?feature=player_embedded&v=odRyv7V8LAE
" target="_blank"><img src="http://img.youtube.com/vi/odRyv7V8LAE/0.jpg" 
alt="YouTube video about the HIV Virus work." width="560" height="315" border="10" /></a>

### Simulation Scales
Here is a list of approximate size scales of use in the simulation that was <a href="http://en.wikibooks.org/wiki/Cell_Biology/Introduction/Cell_size">borrowed from Wikibooks</a>. The sizes don't have to be exact but are meant to serve as a scale reference.

<table>
<tr><td><b>Width(Diameter)</b></td><td><b>Object</b></td></tr>
<tr><td>0.1 nm      </td><td>diameter of a hydrogen atom</td></tr>
<tr><td>0.8 nm      </td><td>Amino Acid</td></tr>
<tr><td>2 nm        </td><td>Diameter of a DNA Alpha helix</td></tr>
<tr><td>4 nm        </td><td>Globular Protein</td></tr>
<tr><td>6 nm        </td><td>microfilaments</td></tr>
<tr><td>7 nm        </td><td>thickness cell membranes</td></tr>
<tr><td>20 nm       </td><td>Ribosome</td></tr>
<tr><td>25 nm       </td><td>Microtubule</td></tr>
<tr><td>30 nm       </td><td>Small virus (Picornaviruses)</td></tr>
<tr><td>30 nm       </td><td>Rhinoviruses</td></tr>
<tr><td>50 nm       </td><td>Nuclear pore</td></tr>
<tr><td>100 nm      </td><td>HIV</td></tr>
<tr><td>120 nm      </td><td>Large virus (Orthomyxoviruses, includes influenza virus)</td></tr>
<tr><td>150-250 nm  </td><td>Very large virus (Rhabdoviruses, Paramyxoviruses)</td></tr>
<tr><td>150-250 nm  </td><td>small bacteria such as Mycoplasma</td></tr>
<tr><td>200 nm      </td><td>Centriole</td></tr>
<tr><td>200-500 nm  </td><td>Lysosomes</td></tr>
<tr><td>200-500 nm  </td><td>Peroxisomes</td></tr>
<tr><td>800 nm      </td><td>giant virus Mimivirus</td></tr>
<tr><td>1 - 10 µm   </td><td>the general sizes for Prokaryotes</td></tr>
<tr><td>1 µm        </td><td>Diameter of human nerve cell process</td></tr>
<tr><td>2 µm        </td><td>E.coli - a bacterium</td></tr>
<tr><td>3 µm        </td><td>Mitochondrion</td></tr>
<tr><td>5 µm        </td><td>length of chloroplast</td></tr>
<tr><td>6 µm        </td><td>(3 - 10 micrometers) the Nucleus</td></tr>
<tr><td>9 µm        </td><td>Human red blood cell</td></tr>
<tr><td>10-30 µm    </td><td>Most Eukaryotic animal cells</td></tr>
<tr><td>10-100 µm   </td><td>Most Eukaryotic plant cells</td></tr>
<tr><td>90 µm       </td><td>Small Amoeba</td></tr>
<tr><td>100 µm      </td><td>Human Egg</td></tr>
<tr><td>160 µm      </td><td>Megakaryocyte</td></tr>
<tr><td>500 µm      </td><td>giant bacterium Thiomargarita</td></tr>
<tr><td>800 µm      </td><td>large Amoeba</td></tr>
<tr><td>1 mm        </td><td>Diameter of the squid giant nerve cell</td></tr>
<tr><td>40mm        </td><td>Diameter of giant amoeba Gromia Sphaerica</td></tr>
<tr><td>120 mm      </td><td>Diameter of an ostrich egg </td></tr>
<tr><td>3 meters    </td><td>Length of a nerve cell of giraffe's neck</td></tr>
</table>

# The License

Copyright 2014-19 (C) Cale Jamison McCollough <<cale@astartup.net>>. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0). Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
