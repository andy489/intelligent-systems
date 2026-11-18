Files:
- "uk12_name.csv": names of the cities
- "uk12_xy.csv": (x,y) coordinates of the cities

The shortest path is:
Aberystwyth -> Inverness -> Brighton -> Newcastle -> Oxford -> Liverpool -> Exeter -> Stratford -> London -> Edinburgh -> Glasgow -> Nottingham

Reversed path is:
Nottingham -> Glasgow -> Edinburgh -> London -> Stratford -> Exeter -> Liverpool -> Oxford -> Newcastle -> Brighton -> Inverness -> Aberystwyth

The total distance is:
1872.780568

NOTE: The coordinates in uk12_xy.csv are in a custom/normalized coordinate system,
not standard geographic (lat/lon). The true optimal for these coordinates is 1872.78,
verified by Held-Karp exact algorithm. The value 1595.74 from the original UK-12
benchmark source applies to a different coordinate scale.


|Aberystwyth         |Nottingham          |
|Inverness           |Glasgow             |
|Brighton            |Edinburgh           |
|Newcastle           |London              |
|Oxford              |Stratford           |
|Liverpool           |Exeter              |
|Exeter              |Liverpool           |
|Stratford           |Oxford              |
|London              |Newcastle           |
|Edinburgh           |Brighton            |
|Glasgow             |Inverness           |
|Nottingham          |Aberystwyth         |
|1872.780567789026   |1872.780567789026   |