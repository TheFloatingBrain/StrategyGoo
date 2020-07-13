# How to play

 - Left click on one of the members of your squad "Squaddie" or "the little blue and gray guys"
 - Select a command from the bottom, only the first and third one work (grenade and move)
 - Right click somewhere on the board for your squaddie to execute the command, You can reassign commands as many times as you would like and assign commands to all our squaddies
 - Press the green checkmark to execute the commands, then the goo will take its turn
 - Have fun, when your squaddies are all eliminated or the goo is all elimanated you win/loose, this state is represented internally.

# Basic Info:

- Turn Based Strategy

- Top Down with merged z and y axis

- Window: 1024 x 768

- Playzone: 768x768

- Side Panel: 256

- Tile Size 64x64

- Camera moves around the map which is large than the Playzone

# Goo: 

Hurts player on contact

## Actions: 

### Move: 

Goo moves a part of itsef towards a player (add tile onto tile closet to player, subtract furthest away tile), if there is an obstical such as a box, it will not hurt the player until 2 turns have passed

### Spread: 

Goo launches a part of itsef which immediatly starts spreading each turn in random directions

### Spreat to player: 

Adds 1 goo point to the player, so long as the player has goo points, the goo continiously does damage too them, for every turn the player does not shake off goo, the goo causes damage

# Squaddie: 

## Actions: 

### Use Weapon: 

#### Grenade Launcher: 
Long range weapon, but not as effective on goo as flamethrower (bonus if time, potential to spread goo by "splatting" it)


#### Flame thrower
Short range weapon but the most effectiv (bonus if have time: catch obsticals on fire)

### Move: 
The player can move in +x, -x, +y, -y directions a certain amount, the length of movement is decreased if moveing onto an obstical. Player looses movment if they have to shake off goo

### Pickup/Place Obstical: 
The player can pick up an obstical with no goo on it (or has had goo on it for only 1 turn) and place it down anywhere except if there is another obstical (bonus if have time: can place obsticals on top of eachother). Question, limit inventory?

### Shake Goo: 
Removes 1 goo point from the player
