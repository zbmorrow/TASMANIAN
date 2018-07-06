function [mat] = tsgReadMatrix(filename, bIsComplex)
%
% [mat] = tsgReadMatrix(filename, bIsComplex)
%
% bIsComplex     :   (optional, default 0)
%                   set to 1 if values are complex
%                   file must have twice as many columns as the second
%                       number in header line if bIsComplex = 1
%                   
% [mat] = tsgReadMatrix(filename, 0)
%
% reads a matrix from a file format
% 
% 3 4
% 1 2 3 4
% 5 6 7 8
% 9 10 11 12
%
% results in the matrix [1 2 3 4; 5 6 7 8; 9 10 11 12;]
%
% [mat] = tsgReadMatrix(filename, 1) reads the file
%
% 3 2
% 1 2 3 4
% 5 6 7 8
% 9 10 11 12
%
% results in the matrix [1+2i 3+4i; 5+6i 7+8i; 9+10i 11+12i]
%

if (~(exist('bIsComplex')) || (~(bIsComplex == 0 || bIsComplex == 1)))
    bIsComplex = 0;
end

fid = fopen(filename);

TSG = fread(fid, [1, 3], '*char');

if (TSG == 'TSG')
    D = fread(fid, [1, 2], '*int');
    Rows = D(1);
    Cols = D(2);
    mat = fread(fid, [Cols, Rows], '*double')';
    fclose(fid);
    if(bIsComplex)
        for j = 1:(Cols/2)
            mat(:,j) = mat(:,2*j-1) + sqrt(-1)*mat(:,2*j);
        end
        mat = mat(:,1:(Cols/2));
    end
    return;
else
    fclose(fid);
end

% not binary format, keep this for old support

fid = fopen(filename);

[s] = fscanf(fid, ' %d ', [1, 2]); % load the number of points

Ni = s(1);
Nj = s(2);

if ((Ni>0) && (Nj>0))
    mat = zeros(Ni, Nj);
else
    mat = [];
end

for i = 1:Ni

    [s] = fscanf(fid, ' %f ', [1, Nj]);
    
    mat(i,:) = s;
    
end

if (bIsComplex)
    for j = 1:(Nj/2)
        mat(:,j) = mat(:,2*j-1) + sqrt(-1)*mat(:,2*j);
    end
    mat = mat(:,1:(Nj/2));
end


fclose(fid);

end
