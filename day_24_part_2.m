syms x y z vx vy vz t1 t2 t3

% Using the top three equations in the input
eqs = [x + t1*vx == 313743533208081 + t1*51,...
       y + t1*vy == 328167831228390 + t1*115,...
       z + t1*vz == 159732064722764 + t1*83,...
       x + t2*vx == 468183773350185 + t2*-243,...
       y + t2*vy == 269960480220160 + t2*-42,...
       z + t2*vz == 439515864552130 + t2*-12,...
       x + t3*vx == 182013004223519 + t3*84,...
       y + t3*vy == 391834672709518 + t3*-172,...
       z + t3*vz == 355278372451916 + t3*15];

sol = solve(eqs, [x y z vx vy vz t1 t2 t3]);
disp(sol.x+sol.y+sol.z);