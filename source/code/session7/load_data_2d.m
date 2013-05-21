function [t,u] = load_data_2d(tstep)
% Usage: [t,u] = load_data_2d(tstep)
%
% Input: tstep is an integer denoting which time step output to load
% 
% Outputs: t is the physical time, and u is the 2D array containing
% the result at the requested time step 
%
% Daniel R. Reynolds
% SMU HPC Workshop
% 20 May 2013

% input general problem information
load u_sol_meta.txt;
nx = u_sol_meta(1);
ny = u_sol_meta(2);
nt = u_sol_meta(3);

% ensure that tstep is allowable
if (tstep < 0 || tstep > nt) 
   error('load_data_2d error: illegal tstep')
end

% set filename string
infile = sprintf('u_sol.%03i.txt',tstep);

% load input file as a long 1-dimensional array
data = load(infile);
      
% separate data array from current time
u1D = data(1:end-1);
t = data(end);

% reshape data into 2D mesh
u = reshape(u1D, [nx, ny]);      

% return with results
return

% end of function
