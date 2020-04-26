import * as readline from 'readline';

let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });

function get(arr : boolean[], x: number, y: number) : boolean {
  if(y < 0 || y >= 5) return false;
  if(x < 0 || x >= 5) return false;
  return arr[y*5 + x];
}

  function count(arr : boolean[], x: number, y: number) : number {
    let retVal = 0;
    for(let dx = -1; dx <= 1; dx += 2) {
      retVal += (get(arr,x+dx,y) ? 1 : 0);
    }
    for(let dy = -1; dy <= 1; dy += 2) {
      retVal += (get(arr,x,y+dy) ? 1 : 0);
    }
    return retVal;
  }

 let arr = [];
 let orig = [];

 function step(arr) : boolean[] {
  let newArray = [];
  for(let y = 0;y < 5;++y) {
    for(let x = 0;x < 5;++x) {
      let cnt = count(arr,x,y);
      if(get(arr,x,y))
      {
        newArray.push(cnt === 1);
      }
      else {
        newArray.push(cnt === 1 || cnt === 2);
      }
    }
  }
  return newArray;
 }

 function display(arr : boolean[]) {
  for(let y = 0;y < 5;++y) {
    let line = '';
    for(let x = 0;x < 5;++x) {
      line += get(arr,x,y) ? '#' : '.'; 
    }
    console.log(line);
  }
 }

rl.on('line',(line : string) => {
    arr = arr.concat(Array.from(line).map(x => x === '#'));    
}); 

function partA() {
  const set = new Set<string>();
  set.add(arr.toString());
  for(;;) {
    arr = step(arr);
    const str = arr.toString();
    if(set.has(str)) {
      break;
    }
    set.add(arr.toString());
  }
  display(arr);
  console.log(arr.reduce((pv, cv, index) => pv + cv * Math.pow(2,index),0));
}



function getKey(x : number, y : number, z : number) : string {
  return `${x},${y},${z}`;
}

function countBDown(dx : number,dy : number,z : number,set : Set<string>) : number {
  let cnt = 0;
  z -= 1;
  if(dy === -1 || dy === 1) {
    if(dx !== 0) {
      throw new Error();
    }
    for(let x = -2;x <= 2;++x) {
      cnt += set.has(getKey(x,dy === -1 ? 2 : -2,z)) ? 1 : 0;
    }
  }
  else {
    if(dy !== 0) {
      throw new Error();
    }
    if(!(dx === -1 || dx === 1)) {
      throw new Error();
    }
    for(let y = -2;y <= 2;++y) {
      cnt += set.has(getKey(dx === -1 ? 2 : -2,y,z)) ? 1 : 0;
    }
  }
  return cnt;
}

function countB(x,y,z,dx,dy,set) : number {
  let nx = x + dx;
  let ny = y + dy;
  let nz = z;
  if(nx == 0 && ny == 0) {
    // we've gone down a level
    return countBDown(dx,dy,z,set);
  }
  else if(nx <= -3 || nx >= 3 || ny <= -3 || ny >= 3) {
    nx = dx;
    ny = dy;
    nz = z+1;
  }
  return set.has(getKey(nx,ny,nz)) ? 1 : 0;
}

function examine(x,y,z,set,newSet) {
    const key = getKey(x,y,z);
    if(newSet.has(key)) {
      return;
    }
    let cnt = 0;
    for(let dy = -1;dy <= 1;dy += 2) {
      cnt += countB(x,y,z,0,dy,set);
    }
    for(let dx = -1;dx <= 1;dx += 2) {
      cnt += countB(x,y,z,dx,0,set);
    }
    if(set.has(key) && cnt === 1)
    {
      newSet.add(key);
    }
    if(!set.has(key) && (cnt === 1 || cnt === 2))
    {
      newSet.add(key);
    }
}

function nextGenDown(dx : number,dy : number,z : number,set : Set<string>,newSet : Set<string>) {
  z -= 1;
  if(dy === -1 || dy === 1) {
    if(dx !== 0) {
      throw new Error();
    }
    for(let x = -2;x <= 2;++x) {
      examine(x,dy === -1 ? 2 : -2,z,set,newSet);
    }
  }
  else {
    if(dy !== 0) {
      throw new Error();
    }
    if(!(dx === -1 || dx === 1)) {
      throw new Error();
    }
    for(let y = -2;y <= 2;++y) {
      examine(dx === -1 ? 2 : -2,y,z,set,newSet);
    }
  }
}

function nextGen(x,y,z,dx,dy,set,newSet) {
  let nx = x + dx;
  let ny = y + dy;
  let nz = z;
  if(nx == 0 && ny == 0) {
    // we've gone down a level
    nextGenDown(dx,dy,z,set,newSet);
    return;
  }
  else if(nx <= -3 || nx >= 3 || ny <= -3 || ny >= 3) {
    nx = dx;
    ny = dy;
    nz = z+1;
  }
  examine(nx,ny,nz,set,newSet);
}

function stepB(set : Set<string>) : Set<string> {
  let newSet = new Set<string>();
  set.forEach(oldBug => {
    let [x,y,z] = oldBug.split(',').map(x => Number(x));
    for(let dy = -1;dy <= 1;dy += 2) {
      nextGen(x,y,z,0,dy,set,newSet);
    }
    for(let dx = -1;dx <= 1;dx += 2) {
      nextGen(x,y,z,dx,0,set,newSet);
    }
  });
  return newSet;
 }

function partB() {
  let set = new Set<string>();
  for(let y = 0;y < 5;++y) {
    for(let x = 0; x < 5;++x) {
      if(orig[y*5+x]) {
        set.add(getKey(x-2,y-2,0));
      }
    }
  }
  for(let n = 0;n < 200;++n) {
    set = stepB(set);
  }
  console.log(`${set.size}`);
}

rl.on('close', () => {
  orig = arr.slice();
  console.log("Part A");
  partA();
  console.log("Part B");
  partB();
});