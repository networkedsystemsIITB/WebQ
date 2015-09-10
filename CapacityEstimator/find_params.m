clear;
initial_theta = [1,1];
lb=[1 1];
ub=[100 100];
%options = optimset('GradObj', 'on', 'MaxIter', 400);
%X=load('file1.txt','txt');
%y=load('file2.txt','txt');
%[theta, cost] = fminunc(@(t)(computeCost(t, X, y)), initial_theta);
%computeCost(initial_theta,X,y);
%X=load('consolidated_inp1.txt','txt');
%cost=computeCost(initial_theta);
options = saoptimset('TolFun',1e-5);

[x,fval,exitFlag,output] = simulannealbnd(@computeCost,initial_theta,lb,ub,options)
%fprintf('Cost at theta found by fminunc: %f\n', cost);
%fprintf('theta: \n');
%fprintf(' %f \n', initial_theta);

