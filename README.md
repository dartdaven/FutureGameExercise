# The Idea
The idea of the project is to create a solid foundation for future games that is proposed to be built on top of it

The design is necessary in order to adhere to the original concept and not compromise with skills.
All items marked with a "-" are literally technical specifications for what needs to be done.
All items marked with a "+" are currently implemented

## Weapon Design:  
\+ There is 2 kinds of weapon (Weapon with Ammo Mechanic, Weapon with Heat Mechanic)  
\+ Each mechanic represents a component  
\+ Every weapon has its own Widget  
\+ Player can take different instances of the same weapon class with different Properties

**Ammo Mechanic Design:**  
\+ When ammo in the rifle equals zero, the weapon does not shoot  
\+ When you press R, ammo from inventory refills lacking ammo in the rifle  
\+ Ammo in the rifle can't be more than 10 (adjustable)  
\+ Ammo in the inventory can't be more than 30 (adjustable)  

**Ammo Widget Design:**  
\+ Widget appears only when you pick up Rifle  
\+ There are 2 numbers on the widget (ammo in the rifle / ammo in the inventory)  
\+ When current ammo becomes less than 40% (adjustable) the text becomes yellow  
\+ When less than 20% (adjustable) the text becomes red and !(-) flickers  

**Heat Mechanic:**  
\+ Shoot while the mouse button is pressed via a certain interval  
\+ Interval between shots can be set via blueprint  
\+ The player can't overcome that interval by simply factor-clicking  
\+ Every shot increases the heat bar by a certain amount  
\+ Temperature is constantly dropping  
\+ When the temperature exceeds a certain amount the weapon becomes overheated  
\+ When the weapon is overheated you can't shoot till the temperature drops down to 20% (adjustable) of the temperature range

**Heat Widget Design:**  
\+ Represents bar of the percentage of current weapon Temperature related to MaxTemperature  
\- When the temperature is over 0.6, the bar becomes yellow, when the temperature is over 0.8, the bar becomes red.  
\+ When the weapon is overheated show "Overheated" text, and hide it when the weapon cools down  

## Grenade design  
\+ Grenade explodes after 3 seconds (adjustible) after been thrown  
\+ When the grenade explodes, it pushes every dynamic object with physics on a scene in a direction away from the grenade  
\+ Grenade can bounce  
\+ When a grenade hits an object, it pushes it  
\+ Grenade don't explode on hit  
\+ Spawn point of the grenade is bound to the Camera  
\+ Grenade can be thrown with different strength  
\+ The more you hold the bottom of Grenade Throthing the more the strength of the throw  
\+ The strength of the throw cannot exceed the cap  
\+ There's a Radial widget that represents the strength of the throw  

## Ammo Collectible:  
\+ You can pick up different types of ammo (ammo, grenade)  
\+ When the collectible type is ammo, there's a text number representing how much ammo is left in the collectible  
\+ You can't take more ammo than fits in your inventory  
\+ The ammo collectible actor is only destroyed when the amount of the ammo becomes 0  
\+ Mesh appearance is chosen during BeginPlay in dependence on what type is set  

## Misc:  
\+ Debug messages by simple function/macro  
\- The aspect ratio of the device does not affect on distance to the weapon  
