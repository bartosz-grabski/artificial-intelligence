// if you want to create levels file of your own make sure it implements:
// 1. global variable levels
// 2. function initLevels() that initializes levels structure
// 3. function nextLevel() that puts into ExpParams.level variable the value of next level's index (or -1 if random) 

// descriptions of game levels
global levels;

function initLevels(){
  // define available levels
  // fields in an level array are:
  // 1: name,
  // 2: world size,
  // 3: world band,
  // 4: world type,
  // 5: world map,
  // 6: number of enemies,
  // 7: enemies collision freeze
  // 8: enemy creature
  // 9: player creature
  
  levels = Vector.new(); 
  // #1
  levels.add([ "You are faster than enemies... Have fun :)", 
               60,                  
               2,
               0,"",
               6,
               1,
               0,
               1]
             );
  // #2
  levels.add([ "Still easy... Now enemies don't freeze..", 
               60,                  
               2,
               0,"",
               8,
               0,
               0,
               1]
             ); 
  // #3
  levels.add([ "Fence! You can't easily escape!", 
               60,                  
               1,
               0,"",
               10,
               1,
               0,
               1]
             );
  // #4
  levels.add([ "Some enemies look just like you... Watch out!", 
               60,                  
               2,
               0,"",
               10,
               1,
               -1,
               1]
             );
  // #5
  levels.add([ "They're all as fast as you!", 
               75,                  
               2,
               0,"",
               10,
               1,
               1,
               1]
             );
  // #6
  levels.add([ "Fence is back!", 
               75,                  
               1,
               0,"",
               12,
               1,
               1,
               1]
             );
  // #7
  levels.add([ "Another breaking the wall...", 
               75,                  
               2,
               1,
               "m 10 10 " +
               "9 9 9 9 5 5 9 9 9 9 " +
               "9 5 5 5 5 5 5 5 5 9 " +
               "9 5 5 5 5 5 5 5 5 9 " +
               "9 5 5 5 5 5 5 5 5 9 " +
               "5 5 5 5 5 5 5 5 5 5 " +
               "5 5 5 5 5 5 5 5 5 5 " +
               "9 5 5 5 5 5 5 5 5 9 " +
               "9 5 5 5 5 5 5 5 5 9 " +
               "9 5 5 5 5 5 5 5 5 9 " +
               "9 9 9 9 5 5 9 9 9 9 ",
               14,
               1,
              -1,
               1]
             );
  
  // #8
  levels.add([ "Hills...", 
               75,                  
               1,
               2,
               "m 10 10 " +
               "5 5 5 6 5 5 6 5 5 5 " +
               "5 5 5 7 5 5 7 5 5 5 " +
               "5 5 7 8 5 5 8 7 5 5 " +
               "6 7 8 9 5 5 9 8 7 6 " +
               "5 5 5 5 5 5 5 5 5 5 " +
               "5 5 5 5 5 5 5 5 5 5 " +
               "6 7 8 9 5 5 9 8 7 6 " +
               "5 5 7 8 5 5 8 7 5 5 " +
               "5 5 5 7 5 5 7 5 5 5 " +
               "5 5 5 6 5 5 6 5 5 5 ",
               12,
               0,
               1,
               1]
             );
}

function nextLevel(){
   if(ExpParams.level == -1) return;
   if(ExpParams.level == levels.size - 1) ExpParams.level = 0;
   else ExpParams.level++;
}
