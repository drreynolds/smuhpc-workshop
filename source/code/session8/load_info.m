function [nx,ny,nt] = load_info()
% Usage: [nx,ny,nt] = load_info()
%
% Outputs: nx,ny are the grid size, and nt is the total number of
% time steps that have been output to disk.
%
% Daniel R. Reynolds
% SMU HPC Workshop
% 20 May 2013

% input general problem information
load u_sol_meta.txt;
nx = u_sol_meta(1);
ny = u_sol_meta(2);
nt = u_sol_meta(3);

return
% end of function
