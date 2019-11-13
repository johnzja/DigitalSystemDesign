clear;clc;close all;
T_inv = [1.4, 1.4065, 1.413, 1.4195, 1.426];
m = length(T_inv);
zeta = zeros(1, m);                             % Initial relative free energy estimations set to 1.
pi_arr = 1/m*ones(1,m);
beta = 0.8;
t0 = 20000;                                     % burn-in size.

q = 10;                                         % status = {1,2, ..., 10}. 10 levels.
K = 400;                                        % 400 particles are under inspection.
szk = sqrt(K);
X = randi([1,q],szk,szk);                       % set state X to a random initial value.
L = randi([1, m], 1,1);                         % set label L to a random initial value.
p_L_arr = zeros(1, m);
accept_cnt = 0;
% Markov move. Global-jump.
% evaluate Global Jump probabilities.
% qj(x) is proportional to exp(-u(x)/T).
N = 40000000;
% thres = round(N/40);

% Record each transition step.
zeta_arr = zeros(N, m);
EL_arr = zeros(N, 2);
E_X = GetEnergy(X);
mean_T_inv = T_inv - mean(T_inv);
% h = waitbar(0, 'Performing MCMC Transition...');
for t = 1:N
%     if mod(N, thres) == 0
%         waitbar(t/N);
%     end
    for var = 1:m
        p_L_arr(var) = pi_arr(var)/exp(zeta(var))*exp(-E_X*mean_T_inv(var));
    end

    p_L_arr1 = p_L_arr / sum(p_L_arr);
    L = sample_from_prob_arr(p_L_arr1);          % Now the label is determined.

%     for iter = 1:400
        coord = randi(szk,[1,2]);               % randomly pick a spin.
        target_level = randi(q);
        [AccProb, Energy_Change] = GetAcceptProb_Once(X, coord, target_level, T_inv(L), szk);
        
        %AccProb = min(1, exp(-EDifference*T_inv(L)));
        x = rand();
        if x < AccProb
            X(coord(1), coord(2)) = target_level;
            E_X = E_X + Energy_Change;
            accept_cnt = accept_cnt + 1;
        end
%     end
    % Update free energies: zeta.
    zeta_subt = zeta;
    zeta_subt(L) = zeta_subt(L) + burn_in(beta, t0, t, pi_arr, L)/pi_arr(L);
    zeta = zeta_subt - zeta_subt(1);
    
%     if E_X ~= GetEnergy(X)
%         error('fuck!');
%     end
    % Do the recording.
    zeta_arr(t, :) = zeta;
    EL_arr(t, 1) = E_X;
    EL_arr(t, 2) = L;
end
% close(h);
% Separate m MARKOV chains.
Sample_indices = -ones(N, m);               % initialize as -1's.
counter = ones(m,1);
for t = 1:N
    m_tmp = EL_arr(t, 2);
    Sample_indices(counter(m_tmp), m_tmp) = t;
    counter(m_tmp) = counter(m_tmp)+1;
end

Sample_Energies = {};

for k = 1:m
    % Energy plot.
    sample_indice_boolean = (Sample_indices(:,k)~=-1);
    sample_cnt = sum(sample_indice_boolean);
    Sample_Energies{k} = zeros(sample_cnt, 1);
    data = Sample_indices(sample_indice_boolean, k);
    data = EL_arr(data, 1);
    Sample_Energies{k} = data;
    figure(k);
    histogram(data,floor((max(data)-min(data)+1)/5));
    
end

for k = 1:m
    figure(m+k);
    plot(zeta_arr(:,k));
end



function t_inv = burn_in(beta, t0, t, pi_arr, j_index)
    if t <= t0
        t_inv = min(pi_arr(j_index), t^(-beta));
    else
        t_inv = min(pi_arr(j_index), 1/(t-t0+t0^beta));
    end
end

% this function should be updated to achieve better computational
% performance.
function [p, energy_change] = GetAcceptProb_Once(s_mat, coord, target_level, T_inv, szk)
    K1 = szk;
    K2 = szk;
    delta_u = 0;
    % Calculate dim0 - energy.
    curr_level = s_mat(coord(1), coord(2));
    if coord(1) == 1
        delta_u = delta_u + (target_level == s_mat(2, coord(2))) - (curr_level == s_mat(2, coord(2)));
        delta_u = delta_u + (target_level == s_mat(K1, coord(2))) - (curr_level == s_mat(K1, coord(2)));
    elseif coord(1) == K1
        delta_u = delta_u + (target_level == s_mat(K1-1, coord(2))) - (curr_level == s_mat(K1-1, coord(2)));
        delta_u = delta_u + (target_level == s_mat(1, coord(2))) - (curr_level == s_mat(1, coord(2)));
    else
        delta_u = delta_u + (target_level == s_mat(coord(1)-1, coord(2))) - (curr_level == s_mat(coord(1)-1, coord(2)));
        delta_u = delta_u + (target_level == s_mat(coord(1)+1, coord(2))) - (curr_level == s_mat(coord(1)+1, coord(2)));
    end
    
    % Calculate dim1 - energy.
    if coord(2) == 1
        delta_u = delta_u +(target_level == s_mat(coord(1), 2)) - (curr_level == s_mat(coord(1), 2));
        delta_u = delta_u + (target_level == s_mat(coord(1), K2)) - (curr_level == s_mat(coord(1), K2));
    elseif coord(2) == K2
        delta_u = delta_u +(target_level == s_mat(coord(1), K2-1)) - (curr_level == s_mat(coord(1), K2-1));
        delta_u = delta_u + (target_level == s_mat(coord(1), 1)) - (curr_level == s_mat(coord(1), 1));
    else
        delta_u = delta_u + (target_level == s_mat(coord(1), coord(2)-1)) - (curr_level == s_mat(coord(1), coord(2)-1));
        delta_u = delta_u + (target_level == s_mat(coord(1), coord(2)+1)) - (curr_level == s_mat(coord(1), coord(2)+1));
    end
    % Validate delta_u.
    delta_u = -delta_u;
%     if 1
%         s_mat_after_preturbation = s_mat;
%         s_mat_after_preturbation(coord(1), coord(2)) = target_level;
%         delta_u_1 = GetEnergy(s_mat_after_preturbation) - GetEnergy(s_mat);
%         if delta_u_1 ~= delta_u
%             error('fuck!');
%         end
%     end
        
    % Calculate Bolzmann factor.
    p = min(1, exp(-delta_u*T_inv));
    energy_change = delta_u;
end

function E = GetEnergy(s_mat)
    [K1, K2] = size(s_mat);
    E = 0;
    for k1 = 1:K1-1
        for k2 = 1:K2
            E = E + (s_mat(k1,k2) == s_mat(k1+1, k2));  % vertical energy.
        end
    end
    for k2 = 1:K2
        E = E + (s_mat(K1, k2) == s_mat(1, k2));
    end
    for k1 = 1:K1
        for k2 = 1:K2-1
            E = E + (s_mat(k1, k2) == s_mat(k1, k2+1)); % horizontal energy.
        end
        E = E + (s_mat(k1, K2) == s_mat(k1, 1));
    end
    E = -E;
end

function L = sample_from_prob_arr(parr)
    x = rand();
    cparr = [0, cumsum(parr)];
    L = find(cparr>=x, 1)-1;
end


