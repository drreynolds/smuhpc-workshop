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
[nx,ny,nt] = load_info();

% ensure that tstep is allowable
if (tstep < 0 || tstep > nt) 
   error('load_data_2d error: illegal tstep')
end

% set filename string and load as a long 1-dimensional array
infile = sprintf('u_sol.%03i.txt',tstep);
data = load(infile);
      
% separate data array from current time, and reshape data into 2D
u1D = data(1:end-1);
t = data(end);
u = reshape(u1D, [nx, ny]);      

return
% end of function
