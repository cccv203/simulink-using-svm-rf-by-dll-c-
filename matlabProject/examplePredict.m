a = importdata('data2.csv');

d1=[];
d2=[];
d3=[];
d4=[];
d5=[];

t1=[];
t2=[];
t3=[];

for i =1:size(a.data,1)
    valid=true;
    for j=1:size(a.data,2)
        if isnan(a.data(i,j))
            valid=false;
            break;
        end
    end
    if(~valid)
        continue;
    end
    d1 = [d1;a.data(i,2)];
    d2 = [d2;a.data(i,5)];
    d3 = [d3;a.data(i,6)];
    d4 = [d4;a.data(i,9)];
    d5 = [d5;a.data(i,10)];
    
    t1 = [t1;a.data(i,3)];
    t2 = [t2;a.data(i,4)];
    t3 = [t3;a.data(i,7)];
end
x = linspace(0,0.25*size(d1,1),size(d1,1));
d1 = [x',d1];  
d2 = [x',d2];
d3 = [x',d3];  
d4 = [x',d4];
d5 = [x',d5]; 
t1 = [x',t1];  
t2 = [x',t2];
t3 = [x',t3];