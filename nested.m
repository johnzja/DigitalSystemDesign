close all;clear;clc;

% 参数设置
ks = 20;                                % 模型参数K
qs = 10;                                % 模型参数q
invT = 1.426;                           % 模型参数1/T（逆温度）
burnin = 20;                          % 舍弃点数
iteras = 120;                        % 迭代次数
steps = 10;                           % 采样步长
samples = iteras-burnin;                % 采样点数

% 初始化
es_seq = zeros(samples,1);             % 采样能量
dx = round(rand(ks,ks,2));
d = sum(sum(sum(dx)));
[sx,cd] = generate_s(dx);
[~,es] = draw_d_from_s(sx);
accepts = 0;

for ts = 1:iteras
    [sxp,cdp] = generate_s(dx);
    [dxp,esp] = draw_d_from_s(sxp);
    dp = sum(sum(sum(dxp)));
    u = rand();
    if(u < exp((dp-d)*log(exp(invT)-1)))
        dx = dxp;
        es = esp;
        sx = sxp;
        cd = cdp;
        d = dp;
        accepts = accepts+1;
    end
    if(mod(ts,steps) == 0)
        es_seq(ts/steps,1) = es;
    end
end

% 绘图
nbins = 40;
histogram(es_seq/(ks*ks),nbins,'Normalization','pdf');
axis([-2,-0.5,0,6]);

% 存储数据
save nested.mat es_seq sx

function [new_s, Cd] = generate_s(d)
    L = 20;                                                 % Assume that size(d) = 20*20*2.
    s_with_cluster_index = zeros(L,L);
    clusters = zeros(L*L+1, L*L)-1;                           % initialize with -1's.
                                                            % Initialize s_with_cluster_index.
    for k1 = 1:L
        for k2 = 1:L
            s_with_cluster_index(k1, k2) = (k1-1)*L+k2;     % Initialize with value:1~400
            clusters(1, (k1-1)*L+k2) = (k1-1)*L+k2;         % Initialize all the cluster classifier.
        end
    end
    % List all the bonds in the d tensor and update union sets.
    for direction = 1:2
        for k1 = 1:L
            for k2 = 1:L
                if d(k1, k2, direction) == 1
                    [s_with_cluster_index, clusters] = update_clusters(s_with_cluster_index, ...
                        clusters, k1, k2, direction);
                end
            end
        end
    end
    
    % Report number of clusters.
    iterator_clusters = find(clusters(1,:)~=-1);
    Cd = sum(clusters(1,:)~=-1);
    new_s = zeros(L,L); 
    
    for iter1 = iterator_clusters
        % Randomly choose a spin for each of the clusters.
        random_spin = randi([1,10]);
        for iter2 = clusters(:,iter1).'
            if iter2 == -1
                break;
            end
            p_k2 = mod(iter2-1, L) + 1;
            p_k1 = (iter2-p_k2)/L + 1;
            new_s(p_k1, p_k2) = random_spin;
        end
    end
end

function [sci, c] = update_clusters(sci_old, c_old, bond_k1, bond_k2, bond_direction)
    % bond_k1: row index; bond_k2: column index. direction: 1 = horizontal.
    % 2 = vertical.
    L = 20;
    point1 = [bond_k1, bond_k2];
    if bond_direction == 1
        if bond_k2 == L
            point2 = [bond_k1, 1];
        else
            point2 = [bond_k1, bond_k2+1];
        end
    else
        if bond_k1 == L
            point2 = [1, bond_k2];
        else
            point2 = [bond_k1+1, bond_k2];
        end
    end
    
    % the elements in sci matrix indicate the index of the cluster to which the lattice
    % point belongs.
    
    class1 = sci_old(point1(1), point1(2)); 
    class2 = sci_old(point2(1), point2(2));
    if class1 ~= class2
        % Step1: Update s-class-index.
        for point_index = c_old(:,class2).'
            if point_index == -1
                break;
            else
                p_k2 = mod(point_index-1, L) + 1;
                p_k1 = (point_index-p_k2)/L + 1;
                sci_old(p_k1, p_k2) = class1;
            end
        end
        
        % Step2: Update clusters. Class2 --> Class1.
        tail_pos = find(c_old(:,class1) == -1, 1, 'first');
        point_index = c_old(:,class2).';
        point_index = point_index(point_index ~=-1);
        for p = point_index
            c_old(tail_pos, class1) = p;
            tail_pos = tail_pos + 1;
        end
        c_old(1, class2) = -1;      % delete class2.
    end
    
    sci = sci_old;
    c = c_old;
end

% 计算能量u(x)
function [dxp,esp] =draw_d_from_s(sx)
[a,b] = size(sx);
mat = [sx,sx(:,1);sx(1,:),0];
dxp = zeros(a,b,2);
esp = 0;
% 只看一个元素与其右、其下的元素是否相同，避免重复
for m = 1:a
    for n = 1:b
        % 横边
        if(mat(m,n) == mat(m+1,n))
            esp = esp+1;
            dxp(m,n,1) = round(rand());
        end
        % 纵边
        if(mat(m,n) == mat(m,n+1))
            esp = esp+1;
            dxp(m,n,2) = round(rand());
        end
    end
end
end
