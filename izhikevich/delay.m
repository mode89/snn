rand('seed',1);

T = 1000;
N = 1000;
Ne = N * 0.8;
Ni = N - Ne;
M = N * 0.1;           % number of synapses per neuron
D=20;                  % maximal conduction delay
a=[0.02*ones(Ne,1);    0.1*ones(Ni,1)];
d=[   8*ones(Ne,1);    2*ones(Ni,1)];

% post=ceil([N*rand(Ne,M);Ne*rand(Ni,M)]);
% Take special care not to have multiple connections between neurons
delays = cell(N,D);
for i=1:Ne
    p=randperm(N);
    post(i,:)=p(1:M);
    for j=1:M
        delays{i, ceil(D*rand)}(end+1) = j;  % Assign random exc delays
    end;
end;
for i=Ne+1:N
    p=randperm(Ne);
    post(i,:)=p(1:M);
    delays{i,1}=1:M;                    % all inh delays are 1 ms.
end;

% synaptic weights
s=[6*ones(Ne,M);-5*ones(Ni,M)];

v = -65*ones(N,1);                      % initial values
u = 0.2.*v;                             % initial values
firings=[];                         % spike timings

for t=1:T                          % simulation of 1 sec
    t

    I=zeros(N,1);
    I(ceil(N*rand))=20;                 % random thalamic input

    fired = find(v>=30);                % indices of fired neurons

    v(fired)=-65;
    u(fired)=u(fired)+d(fired);

    firings=[firings;t*ones(length(fired),1),fired];
    k=size(firings,1);
    while k >= 1 && firings(k,1)>t-D
        del=delays{firings(k,2),t-firings(k,1)+1};
        ind = post(firings(k,2),del);
        I(ind) = I(ind) + s(firings(k,2), del)';
        k=k-1;
    end;

    v=v+0.5*((0.04*v+5).*v+140-u+I);    % for numerical
    v=v+0.5*((0.04*v+5).*v+140-u+I);    % stability time
    u=u+a.*(0.2*v-u);                   % step is 0.5 ms
end;

plot(firings(:,1),firings(:,2),'.');
axis([0 T 0 N]); drawnow;
pause
