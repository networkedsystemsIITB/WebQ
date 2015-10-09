function cost=computeCost(theta)
	%m=0.9;
	%n=0.1;
    theta
    %X=load('consolidated_inp.txt','txt');
    X=load('capacityEstimator.inp','txt');
	%wt=X(:,7)*theta(1)+X(:,8)*theta(2);
	wt=(X(:,1)./(X(:,1)+X(:,2)))*theta(1)+(X(:,2)./(X(:,1)+X(:,2)))*theta(2);
	%X(:,1)=X(:,1).*X(:,7)*theta(1)+X(:,1).*X(:,8)*theta(2);
	X(:,1)=X(:,1)*theta(1)+X(:,2)*theta(2);
	X(:,3)=X(:,3)*theta(1);
	X(:,4)=X(:,4)*theta(2);
	X(:,5)=X(:,5)./wt;
	t1=(X(:,3)+X(:,4))./X(:,5);
	t2=t1(t1>2000);
	
	X1=X((t1>2000),1);
	X2=X((t1>2000),[1 6]);
	X3=X(:,[1 6]);
	%t3=[X1.^2 X1 ones(size(t2,1),1)];
    t3=[X1.^3 X1.^2 X1 ones(size(t2,1),1)];
	a=pinv(t3)*t2;
	e=norm(t2-t3*a);
    if(a(1)==0 && a(2)==0 && a(3)==0)
        cost=1000000;
    else
        cost=e
    end
	
 	sortedX2=sortrows(X3,2);
	[A,B,uniqueIndex]=unique(sortedX2(:,2));
	cellA=mat2cell(sortedX2,accumarray(uniqueIndex(:),1),2);
	
	xmax=max(X3(:,1));
	x=0:1:150;
	%y2=a(1)*x.^2+a(2)*x+a(3);
	y2=a(1)*x.^3+a(2)*x.^2+a(3)*x+a(4);
	hold off;	
	
	start=1;
% 	
% 	ymax=max(t2);
% 	C=[1 0 0;
% 		0 1 0;
% 		0.0 0.0 1;
% 		1 1 0;
% 		0 1 1];
% 	for i=1:size(cellA,1)
% 		t4=cell2mat(cellA(i));
% 		t4=t4(:,1);
% 		
% 		t5=t1(start:(start+size(t4,1)-1));
% 		%scatter(t4',t5',6,C(i,:));
%         scatter(t4',t5',6);
% 		
% 		hold on;
% 		start=start+size(t4,1);
% 	end
	scatter(X1,t2);
    hold on;
	title('Scaled Plot');
	xlabel('scaled arrival rate(req/sec)');
	ylabel('scaled power ratio');
	
	plot(x,y2,'k--');

	capacity=-a(2)/(2*a(1));
	fileID=fopen('capacity.txt','w');
	fprintf(fileID,'%f',capacity);
end
