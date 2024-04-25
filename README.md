# GameOfLife
Implementation of Conway’s Game of Life, extended with custom species.
The basic rules can be seen in: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

The additional behavior of the different species can be seen in the table below.

| Species   | Needs | Overpopulated by | Takes over | Overwritten by | Specialty |
| ------------- | ------------- |------------- |------------- |------------- |------------- |
| Default   | same  | same              | none        | none |
| White     | same  | same              | none        | all  | Invisible to others
| Red       | same  | all               | --          | none | Kills foreign neighbors
| Green     | all   | all               | none        | none |
| Blue      | same  | all               | none        | none | Reaction speed x0.5
| Purple    | same  | all               | instant     | none | Infects neighbors
| Orange    | same  | all               | none        | none |
| Yellow    | same  | all               | none        | none |
