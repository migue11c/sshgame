# (not actually) sshgame
tui game that isnt actually played over ssh, that was just the initial name, it will undergo changes

done:
  - main and login menus
  - auth system (without hash)
  - centered text animatin
  - player database
  - settings menu
  - (almost) loading screen
  - vector rendering and scaling

next tasks:

1. networking (almost, needs a timeout and a stopwatch for testing, also needs method for sending save data)
2. polished raster rendering with controls
3. file checks for ifstream (i VERY LIKELY won't need this, except for backup save)
4. selective map controls for city map
5. dialogue system (with checks)
6. combat system
7. item system (job is started but needs more work)

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
zoom 2 will also draw an additional border between nest and the backstreets
