# sshgame
tui game that isnt actually played over ssh, that was just the initial name

done:
  - main and login menus
  - basic map rendering
  - auth system (without hash)
  - centered text animatin
  - player database

next tasks:

1. networking
2. fix newline and eof when rendering with std::getline
3. add points of interest on the map so it can render with those in the middle
  when adding points of interest, make sure that camera does not go out of bounds.
DONE settings menu with actual settings
5. file checks for ifstream
DONE audio (cross platform)
7. segmenting the map and rendering each part seperately
  (this is so that the map segments can be animated)
MOSTLY DONE loading screen (multithreading)

The whole article regarding map rendering is kept as a reference if i change my mind.

Map will have 3 different zooms. They will all have an animated transition.
The map will be drawn with vectors. X and Y of every vertex will be float or double (depending on the need).
To render further away and closer, the xy of each vector will have a scale multiplier.
To simplify the workflow, there will also be a camera function that has its' own coordinates and scales xy coords too.

To summarize, the map is being stretched and squished, and camera position will accomodate for it.

the map coordinates is being accounted for the default powershell font: Consolas: 16 pixels tall and 8 pixels wide, so x coordinate will be twice as long


enter will zoom in, backspace will zoom out
zoom 0 will show the entire map and arrowkeys will move the camera in the direction of the keypress.

zoom 1 will show the districts along with their danger levels. districts will have a filled area and a border seperate.
some areas will have their fill animated depending on the situation.
the camera will move to the selected district.
the district the player is currently in will have [x] on top of it, others will have [ ]
the district the player is selecting will have:
  - animated text above [ ] at the time of selection naming the district
  - cost of moving below [ ] that's blinking (example "5 hrs | 800 won")


district selection will be tough to implement but a temporary solution will be just scrolling through an array of positions

zoom 2 will draw additional rasters over the fill, depending on the map
zoom 2 will also draw an additional border between nest and the









designing map render:

the render will have 2 zooms:
zoom 0 will render multiple maps in one screen, reason to do this is to animate certain ones while they're in a certain state
for this we need to introduce worldData struct to interact with the map render

each map on the zoom 0 will have its' coordinates.
camera will take playerData and set its' relative coordinates to the location the player is in
the areas the player can travel to will be highlighted (this is determined with how much time you have left in your day)
you can use arrowkeys to select a district you want to zoom in to, enter to confirm
moving selections will move the camera
moving selections will cause animated text to appear over that district.

map sections will have each their own file they are drawn from.
zoom 0 maps will be in maps/zoom0/mapxx.map
zoom 1 maps will be in maps/zoom1/mapxx.map

for my campagin this can be sorted into an array of maps Map[25][2]
where maps is a struct with the properties:
  int id;
  int type;

this is to simplify the use of the struct.
you ask for maps.id and maps.type and append it to an array from worldInfo.dat (downloaded from server)
worldInfo.dat is where you get the map names.

other map data will be in maps/dataname/mapxx.map

zoom 1 will only display one map and points of interest like [x]
hidden points of interest will not be visible until you have fulfilled a condition.
the condition can be you owning an item or beating a certain encounter with a certain ending
(when you do that you will recieve an invisible item, to simplify the code)
arrowkeys will show animated text over the point of interest and move the camera towards it a bit.

selecting a point of interest will grant you a dialogue, and options. some options will be unavaliable (greyed out, or marked as such)
for an option to be unavaliable there has to be an unlock condition. certain day, time of day or certain item held.
unavaliable option will be able to be selected but not confirmed.

the real challenge will be the games you play after you select the levels (if you are designing this game to entirely rely on downloading from the server)
