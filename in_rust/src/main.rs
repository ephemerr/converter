use std::collections::HashMap;
use std::io;
use std::io::BufRead;
use std::time::Instant;

struct Unit {
  chain: u32,
  proportion: f64
}


pub fn main() -> io::Result<()> {
  let mut unit_sys : HashMap<String, Unit> = HashMap::with_capacity(2000);
  let mut chain_count = 0;
  let stdin = std::io::stdin();
  let now = Instant::now(); 

  for line in stdin.lock().lines() {
    let line = line.unwrap();
    let words: Vec<&str> = line.split(" ").collect();
    let mut new_units : Vec<(String, Unit)> = Vec::new();

    if words[3].to_string() != "?" {
      let v1 : f64 = words[0].parse().unwrap();
      let u1 = words[1];
      let v2 : f64 = words[3].parse().unwrap();
      let u2 = words[4];
      // println!("{} {} = {} {}",v1,u1,v2,u2);
      {
        let e1 = unit_sys.get(u1);
        let e2 = unit_sys.get(u2);

        match e1 {
          Some(unit1) => {
            match e2 {
              Some(_unit2) => (),
              None => {
                new_units.push((
                    u2.to_string(),
                    Unit {chain: unit1.chain, proportion: v2/v1*unit1.proportion}
                ));
              }
            }
          }
          None => {
            match e2 {
              Some(unit2) => {
                new_units.push((
                    u1.to_string(),
                    Unit {chain: unit2.chain, proportion: v1/v2*unit2.proportion}
                ));
              },
              None => {
                chain_count = chain_count + 1;
                new_units.push((
                    u1.to_string(),
                    Unit {chain: chain_count-1, proportion: 1.}
                ));
                new_units.push((
                    u2.to_string(),
                    Unit {chain: chain_count-1, proportion: v2/v1}
                ));
              }
            }
          }
        }
      }
      for (name, unit) in new_units {
        unit_sys.insert(name,unit);
      }
    } else {
      let v1 : f64 = words[0].parse().unwrap();
      let u1 = words[1];
      let u2 = words[4];
      let e1 = unit_sys.get(u1);
      let e2 = unit_sys.get(u2);
      match e1 {
        Some(unit1) => {
          match e2 {
            Some(unit2) => {
              if unit1.chain != unit2.chain {
                println!("No conversion is possible.")
              } else {
                let v2 : f64 = v1/unit1.proportion*unit2.proportion;
                let s1 = v1 >=1000000. || v1 <0.1;
                let s2 = v2 >=1000000. || v2 <0.1;
                if !s1 && !s2     { println!("{:.6} {} = {:.6} {}",v1,u1,v2,u2);  }
                else if s1 && !s2 { println!("{:.6e} {} = {:.6} {}",v1,u1,v2,u2); }
                else if !s1 && s2 { println!("{:.6} {} = {:.6e} {}",v1,u1,v2,u2); }
                else if s1 && s2  { println!("{:.6e} {} = {:.6e} {}",v1,u1,v2,u2);}
              }
            }
            None => println!("No conversion is possible.")
          }
        },
        None => println!("No conversion is possible.")
      }
    }
  }

  let elapsed = now.elapsed();
  println!("Elapsed: {}", elapsed.as_micros());

  Ok(())
}
