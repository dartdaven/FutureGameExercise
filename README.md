# The Idea
The idea of the project is to create a solid foundation for future games that is proposed to be built on top of it

The design is necessary in order to adhere to the original concept and not compromise with skills.
All items marked with a "-" are literally technical specifications for what needs to be done.
All items marked with a "+" are currently implemented

## Weapon Design:  
\+ There will be 2 kinds of weapon (Weapon with Ammo Mechanic, Weapon with Heat Mechanic)  
\+ Each mechanic represents a component  
\+ Every weapon kind contains its own Widget

**Ammo Mechanic Design:**  
\+ When ammo in the rifle equals zero, the weapon does not shoot  
\+ When you press R, ammo from inventory refills lacking ammo in the rifle  
\+ Ammo in the rifle can't be more than 10
\+ Ammo in the inventory can't be more than 30

**Ammo Widget Design:**  
\+ Widget appears only when you pick up Rifle  
\+ There are 2 numbers on the widget (ammo in the rifle / ammo in the inventory)  
\+ When current ammo becomes 5 the text becomes yellow  
\+ When it becomes 2 the text becomes red and flickers

**Heat Mechanic:**
\+ Shoot while the mouse button is pressed via a certain interval  
\+ Interval between shots can be set via blueprint  
\+ The player can't overcome that interval by simply factor-clicking  
\+ Every shot increases the heat bar by a certain amount  
\+ Temperature is constantly dropping  
\+ When the temperature exceeds a certain amount the weapon becomes overheated  
\+ When the weapon is overheated you can't shoot till the temperature drops down to 20% of the temperature range

**Heat Widget Design:**  
\+ Represents bar of the percentage of current weapon Temperature related to MaxTemperature  
\- When the temperature is over 0.6, the bar becomes yellow, when the temperature is over 0.8, the bar becomes red.  
\+ When the weapon is overheated show "Overheated" text, and hide it when overheated cools down  

## Grenade design  
\- Grenade has consumable type and is not considered weapon  
\- Grenade is to be thrown by separate button  
\- Explodes after some time after being thrown  
\- When explodes has blast wave that pushes objects around

## Inventory design  
\- collectibles have different types (consumables, weapons)  
\- non inventory items should not be stored in other actors that has no inventory component  
\- player should bw able to switch between weapon types
\- every invetory item has it's limit 

## Collectibles design  
\+ text of amount of consumables is always faced to the player  
\+ collectibles are given to player in amount player can carry 
\+ collectible actor is only destroyed when the amount of the collectible becomes 0
