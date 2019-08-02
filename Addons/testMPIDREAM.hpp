/*
 * Copyright (c) 2017, Miroslav Stoyanov
 *
 * This file is part of
 * Toolkit for Adaptive Stochastic Modeling And Non-Intrusive ApproximatioN: TASMANIAN
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
 *    and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse
 *    or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * UT-BATTELLE, LLC AND THE UNITED STATES GOVERNMENT MAKE NO REPRESENTATIONS AND DISCLAIM ALL WARRANTIES, BOTH EXPRESSED AND IMPLIED.
 * THERE ARE NO EXPRESS OR IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE SOFTWARE WILL NOT INFRINGE ANY PATENT,
 * COPYRIGHT, TRADEMARK, OR OTHER PROPRIETARY RIGHTS, OR THAT THE SOFTWARE WILL ACCOMPLISH THE INTENDED RESULTS OR THAT THE SOFTWARE OR ITS USE WILL NOT RESULT IN INJURY OR DAMAGE.
 * THE USER ASSUMES RESPONSIBILITY FOR ALL LIABILITIES, PENALTIES, FINES, CLAIMS, CAUSES OF ACTION, AND COSTS AND EXPENSES, CAUSED BY, RESULTING FROM OR ARISING OUT OF,
 * IN WHOLE OR IN PART THE USE, STORAGE OR DISPOSAL OF THE SOFTWARE.
 */

#include "TasmanianAddons.hpp"
#include "tasgridCLICommon.hpp"

inline bool testLikelySendRecv(){
    int me = TasGrid::getMPIRank(MPI_COMM_WORLD);
    TasDREAM::LikelihoodGaussIsotropic ref_isolike(10.0, {1.0, 2.0, 3.0});

    if (me == 0){
        if (TasDREAM::MPILikelihoodSend(ref_isolike, 1, 11, MPI_COMM_WORLD) != MPI_SUCCESS) return false;
    }else if (me == 1){
        TasDREAM::LikelihoodGaussIsotropic isolike;
        if (TasDREAM::MPILikelihoodRecv(isolike, 0, 11, MPI_COMM_WORLD) != MPI_SUCCESS) return false;
        std::vector<double> model = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}; // full rank matrix to cover all entries
        std::vector<double> result(3), true_result(3);
        isolike.getLikelihood(TasDREAM::logform, model, result);
        ref_isolike.getLikelihood(TasDREAM::logform, model, true_result);
        for(size_t i=0; i<3; i++) if (std::abs(result[i] - true_result[i]) > TasGrid::Maths::num_tol) return false;
    }

    TasDREAM::LikelihoodGaussAnisotropic ref_alike({4.0, 5.0, 6.0}, {1.0, 2.0, 3.0});

    if (me == 1){
        if (TasDREAM::MPILikelihoodSend(ref_alike, 2, 12, MPI_COMM_WORLD) != MPI_SUCCESS) return false;
    }else if (me == 2){
        TasDREAM::LikelihoodGaussAnisotropic alike;
        if (TasDREAM::MPILikelihoodRecv(alike, 1, 12, MPI_COMM_WORLD) != MPI_SUCCESS) return false;
        std::vector<double> model = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}; // full rank matrix to cover all entries
        std::vector<double> result(3), true_result(3);
        alike.getLikelihood(TasDREAM::logform, model, result);
        ref_alike.getLikelihood(TasDREAM::logform, model, true_result);
        for(size_t i=0; i<3; i++) if (std::abs(result[i] - true_result[i]) > TasGrid::Maths::num_tol) return false;
    }

    return true;
}
