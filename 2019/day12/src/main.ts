  
  class Vec3 {
    public x : number;
    public y : number;
    public z : number;

    constructor(x: number = 0,y:number = 0,z:number = 0) {
      this.x =x;
      this.y = y;
      this.z = z;
    }

    add(v: Vec3) {
      this.x += v.x;
      this.y += v.y;
      this.z += v.z;
    }

    getString() : string {
      return `<x=${this.x}, y=${this.y}, z=${this.z}>`;
    }

    getAbsSum() : number {
      return Math.abs(this.x) + Math.abs(this.y) + Math.abs(this.z);
    }
  }

  class Moon {
    public pos : Vec3;
    public vel : Vec3;

    constructor(x : number,y: number,z: number) {
      this.pos = new Vec3(x,y,z);
      this.vel = new Vec3();
    }

    print() {
      console.log(`pos=${this.pos.getString()}, vel=${this.vel.getString()}`);
    }

    getTotalEnergy() : number {
      return this.pos.getAbsSum() * this.vel.getAbsSum();
    }
  }

  let moons = [ new Moon(17,-7,-11),
    new Moon(1,4,-1),
   new Moon(6,-2,-6),
 new Moon(19,11,9)];


/*
let moons = [ new Moon(-1, 0, 2),
  new Moon(2,-10,-7),
  new Moon(4,-8,8),
  new Moon(3,5,-1)];
*/
  function updateVelocity() {
    for(let i = 0;i < moons.length;++i) {
      let mi = moons[i];
      for(let j = i+1;j < moons.length;++j) {
        let mj = moons[j];
        if(mi.pos.x < mj.pos.x) {
          mi.vel.x += 1;
          mj.vel.x -= 1;
        } else if(mi.pos.x > mj.pos.x) {
          mi.vel.x -= 1;
          mj.vel.x += 1;
        }

        if(mi.pos.y < mj.pos.y) {
          mi.vel.y += 1;
          mj.vel.y -= 1;
        } else if(mi.pos.y > mj.pos.y) {
          mi.vel.y -= 1;
          mj.vel.y += 1;
        }

        if(mi.pos.z < mj.pos.z) {
          mi.vel.z += 1;
          mj.vel.z -= 1;
        } else if(mi.pos.z > mj.pos.z) {
          mi.vel.z -= 1;
          mj.vel.z += 1;
        }
      }
    }
  }

  function updatePositions() {
    moons.forEach(m => {
      m.pos.add(m.vel);
    })
  }

  function doStep() {
    updateVelocity();
    updatePositions();
  }


  function getTotalEnergy() : number {
    return moons.reduce((cv,m) => cv + m.getTotalEnergy(),0);
  }

  function partA()
  {
    for(let i = 0;i < 1000;++i) {
      doStep();
    }
    console.log(getTotalEnergy());
  }

  function gcd(x : number, y : number) : number {
    x = Math.abs(x);
    y = Math.abs(y);
    while(y) {
      let t = y;
      y = x % y;
      x = t;
    }
    return x;
  }

  function lcm(x : number,y : number) : number {
    return (x * y) / gcd(x,y);
  }

  function partB() {
    
    let xLoc = [];
    let yLoc = [];
    let zLoc = [];
    let xVel = [];
    let yVel = [];
    let zVel = [];
    moons.forEach(m => {
      xLoc.push(m.pos.x);
      xVel.push(m.vel.x);
      yLoc.push(m.pos.y);
      yVel.push(m.vel.y);
      zLoc.push(m.pos.z);
      zVel.push(m.vel.z);
    })
    
    let xCycle = null;
    let yCycle = null;
    let zCycle = null;
    let numSteps = 0;
    while(xCycle == null || yCycle == null || zCycle == null) {
      doStep();
      numSteps += 1;
      if(xCycle == null) {
          if(moons.every((m,i) => m.pos.x === xLoc[i] && m.vel.x === xVel[i])) {
            xCycle = numSteps;
          }
      }
      if(yCycle == null) {
        if(moons.every((m,i) => m.pos.y === yLoc[i] && m.vel.y === yVel[i])) {
          yCycle = numSteps;
        }
      }
      if(zCycle == null) {
        if(moons.every((m,i) => m.pos.z === zLoc[i] && m.vel.z === zVel[i])) {
          zCycle = numSteps;
        }
      }
    }
    let periods = [xCycle, yCycle, zCycle];
    periods.forEach(p => console.log(p));
    console.log(periods.reduce((pv,v)=> lcm(pv,v),1));
  }

  try {
    partA();
    partB();
  } catch(e) {
    console.log(e);
  }