% Plotting script for 2D acoustic wave propagation example
% simulation.  This script inputs the file u_sol_meta.txt to determine
% simulation information (grid size and total number of time steps).
% It then calls load_data_2d() to read the solution data from each
% time step, plotting the results (and saving them to disk).
%
% Daniel R. Reynolds
% SMU HPC Workshop
% 20 May 2013
clear

% input general problem information
[nx,ny,nt] = load_info();

% loop over time steps
for tstep = 0:nt

   % load time step data
   [t,u] = load_data_2d(tstep);

   % plot current solution (and save to disk)
   xvals = linspace(0,1,nx);
   yvals = linspace(0,1,ny);
   h = surf(yvals,xvals,u);
   shading flat
   view([50 44])
   axis([0, 1, 0, 1, -1, 1])
   xlabel('x','FontSize',14), ylabel('y','FontSize',14)
   title(sprintf('u(x,y) at t = %g, mesh = %ix%i',t,nx,ny),'FontSize',14)
   pfile = sprintf('u_surf.%03i.png',tstep);
   saveas(h,pfile);
   
   %disp('pausing: hit enter to continue')
   %pause
end

% end of script


