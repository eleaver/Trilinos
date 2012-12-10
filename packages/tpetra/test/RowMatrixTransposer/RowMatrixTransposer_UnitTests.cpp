/*
// @HEADER
// ***********************************************************************
//
//          Tpetra: Templated Linear Algebra Services Package
//                 Copyright (2008) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Michael A. Heroux (maherou@sandia.gov)
//
// ************************************************************************
// @HEADER
*/

#include <Tpetra_TestingUtilities.hpp>

#include <Tpetra_CrsMatrix.hpp>
#include <TpetraExt_MatrixMatrix.hpp>
#include <MatrixMarket_Tpetra.hpp>
#include <Tpetra_RowMatrixTransposer.hpp>

namespace {

  using Tpetra::TestingUtilities::getNode;
  using Tpetra::TestingUtilities::getDefaultComm;

  using Teuchos::RCP;
  using Teuchos::rcp;
  using Teuchos::OrdinalTraits;
  using Teuchos::ScalarTraits;
  using Teuchos::Comm;
  using Teuchos::Array;
  using Teuchos::ArrayView;
  using Teuchos::tuple;
  using Teuchos::null;

  using Tpetra::Map;
  using Tpetra::DefaultPlatform;
  using Tpetra::CrsMatrix;
  using Tpetra::createCrsMatrix;
  using Tpetra::createUniformContigMapWithNode;
  using Tpetra::MatrixMarket::Reader;
  using Tpetra::MatrixMatrix::Add;
  using Tpetra::RowMatrixTransposer;


  TEUCHOS_STATIC_SETUP()
  {
    Teuchos::CommandLineProcessor &clp = Teuchos::UnitTestRepository::getCLP();
    clp.addOutputSetupOptions(true);
    clp.setOption(
        "test-mpi", "test-serial", &Tpetra::TestingUtilities::testMpi,
        "Test MPI (if available) or force test of serial.  In a serial build,"
        " this option is ignored and a serial comm is always used." );
  }

  TEUCHOS_UNIT_TEST_TEMPLATE_4_DECL( RowMatrixTransposer, RectangularTranspose, LO, GO, Scalar, Node )
  {
    RCP<Node> node = getNode<Node>();
    typedef ScalarTraits<Scalar> ST;
    typedef OrdinalTraits<LO> LOT;
    typedef CrsMatrix<Scalar,LO,GO,Node> MAT;
    // get a comm
    RCP<const Comm<int> > comm = getDefaultComm();
    int numProcs = comm->getSize();
    Tpetra::global_size_t numGlobal = 4*numProcs;

    RCP<const Map<LO,GO,Node> > rowMap = createUniformContigMapWithNode<LO,GO>(numGlobal,comm,node);

    RCP<MAT> matrix = Reader<MAT>::readSparseFile("a.mtx", comm, node);
    RCP<MAT> matrixT = Reader<MAT>::readSparseFile("atrans.mtx", comm, node);

    RowMatrixTransposer<Scalar, LO, GO, Node> at(*matrix);
    RCP<MAT> calculated = at.createTranspose();

    RCP<MAT> diffMatrix = rcp(new MAT(matrixT->getRowMap(), matrixT->getNodeMaxNumRowEntries()));

    Scalar sOne = ScalarTraits<Scalar>::one();
    Add(*calculated, false, -sOne, *matrixT, false, sOne, diffMatrix);
    diffMatrix->fillComplete(matrixT->getDomainMap(), matrixT->getRangeMap());

    Scalar diffNorm = diffMatrix->getFrobeniusNorm();
    Scalar realNorm = matrixT->getFrobeniusNorm();
    Scalar epsilon = diffNorm/realNorm;

    TEST_COMPARE(ScalarTraits<Scalar>::real(epsilon), <, 1e-10)
    TEST_COMPARE(ScalarTraits<Scalar>::imag(epsilon), <, 1e-10)

  }


//Not doing complex since we don't have a complex matrix to test on
//typedef std::complex<float>  ComplexFloat;
//typedef std::complex<double> ComplexDouble;

#define UNIT_TEST_GROUP_ORDINAL_SCALAR_NODE( LO, GO, SCALAR, NODE ) \
      TEUCHOS_UNIT_TEST_TEMPLATE_4_INSTANT( RowMatrixTransposer, RectangularTranspose, LO, GO, SCALAR, NODE )  \





#define UNIT_TEST_SERIALNODE(LO, GO, SCALAR) \
      UNIT_TEST_GROUP_ORDINAL_SCALAR_NODE( LO, GO, SCALAR, SerialNode )

typedef Kokkos::DefaultNode::DefaultNodeType DefaultNode;
#define UNIT_TEST_DEFAULTNODE(LO, GO, SCALAR) \
      UNIT_TEST_GROUP_ORDINAL_SCALAR_NODE( LO, GO, SCALAR, DefaultNode )

#ifdef HAVE_KOKKOSCLASSIC_TBB
#define UNIT_TEST_TBBNODE(LO, GO, SCALAR) \
      UNIT_TEST_GROUP_ORDINAL_SCALAR_NODE( LO, GO, SCALAR, TBBNode )
#else
#define UNIT_TEST_TBBNODE(LO, GO, SCALAR)
#endif

#ifdef HAVE_KOKKOSCLASSIC_THREADPOOL
#define UNIT_TEST_TPINODE(LO, GO, SCALAR) \
      UNIT_TEST_GROUP_ORDINAL_SCALAR_NODE( LO, GO, SCALAR, TPINode )
#else
#define UNIT_TEST_TPINODE(LO, GO, SCALAR)
#endif

// don't test Kokkos node for MPI builds, because we probably don't have multiple GPUs per node
#if defined(HAVE_KOKKOSCLASSIC_CUSPARSE) && !defined(HAVE_TPETRA_MPI)
// float
#if defined(HAVE_KOKKOSCLASSIC_CUDA_FLOAT)
#  define UNIT_TEST_THRUSTGPUNODE_FLOAT(LO, GO) \
          UNIT_TEST_GROUP_ORDINAL_SCALAR_NODE( LO, GO, float, ThrustGPUNode )
#else
#  define UNIT_TEST_THRUSTGPUNODE_FLOAT(LO, GO)
#endif
// double
#if defined(HAVE_KOKKOSCLASSIC_CUDA_DOUBLE)
#  define UNIT_TEST_THRUSTGPUNODE_DOUBLE(LO, GO) \
          UNIT_TEST_GROUP_ORDINAL_SCALAR_NODE( LO, GO, double, ThrustGPUNode )
#else
#  define UNIT_TEST_THRUSTGPUNODE_DOUBLE(LO, GO)
#endif
#else
// none
# define UNIT_TEST_THRUSTGPUNODE_FLOAT(LO, GO)
# define UNIT_TEST_THRUSTGPUNODE_DOUBLE(LO, GO)
# define UNIT_TEST_THRUSTGPUNODE_COMPLEX_FLOAT(LO, GO)
# define UNIT_TEST_THRUSTGPUNODE_COMPLEX_DOUBLE(LO, GO)
#endif

#define UNIT_TEST_ALLCPUNODES(LO, GO, SCALAR) \
    UNIT_TEST_DEFAULTNODE(LO, GO, SCALAR)

#define UNIT_TEST_FLOAT(LO, GO) \
    UNIT_TEST_ALLCPUNODES(LO, GO, float) \
    UNIT_TEST_THRUSTGPUNODE_FLOAT(LO, GO)

#define UNIT_TEST_DOUBLE(LO, GO) \
    UNIT_TEST_ALLCPUNODES(LO, GO, double) \
    UNIT_TEST_THRUSTGPUNODE_DOUBLE(LO, GO)


#if defined(HAVE_TPETRA_INST_DOUBLE)
  UNIT_TEST_DOUBLE(int, int)
#endif

}
