// @HEADER
//
// ***********************************************************************
//
//             Xpetra: A linear algebra interface package
//                  Copyright 2012 Sandia Corporation
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
// Questions? Contact
//                    Jonathan Hu       (jhu@sandia.gov)
//                    Andrey Prokopenko (aprokop@sandia.gov)
//                    Ray Tuminaro      (rstumin@sandia.gov)
//
// ***********************************************************************
//
// @HEADER
#ifndef XPETRA_EPETRACRSMATRIX_HPP
#define XPETRA_EPETRACRSMATRIX_HPP

/* this file is automatically generated - do not edit (see script/epetra.py) */

#include "Xpetra_EpetraConfigDefs.hpp"

#include "Xpetra_CrsMatrix.hpp"

#include <Epetra_CrsMatrix.h>
#include <Epetra_Map.h>

#include "Xpetra_EpetraMap.hpp"
#include "Xpetra_EpetraVector.hpp"
#include "Xpetra_EpetraMultiVector.hpp"
#include "Xpetra_EpetraCrsGraph.hpp"

#include "Xpetra_Utils.hpp"
#include "Xpetra_Exceptions.hpp"

namespace Xpetra {

  template<class EpetraGlobalOrdinal>
  class EpetraCrsMatrixT
    : public CrsMatrix<double, int, EpetraGlobalOrdinal>
  {
    typedef EpetraGlobalOrdinal GlobalOrdinal;
    typedef CrsMatrix<double, int, GlobalOrdinal>::scalar_type Scalar;
    typedef CrsMatrix<double, int, GlobalOrdinal>::local_ordinal_type LocalOrdinal;
    typedef CrsMatrix<double, int, GlobalOrdinal>::global_ordinal_type GlobalOrdinal;
    typedef CrsMatrix<double, int, GlobalOrdinal>::node_type Node;

  public:

    //! @name Constructor/Destructor Methods
    //@{

    //! Constructor specifying fixed number of entries for each row.
    EpetraCrsMatrixT(const Teuchos::RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &rowMap, size_t maxNumEntriesPerRow, ProfileType pftype=DynamicProfile, const Teuchos::RCP< Teuchos::ParameterList > &params=Teuchos::null);

    //! Constructor specifying (possibly different) number of entries in each row.
    EpetraCrsMatrixT(const Teuchos::RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &rowMap, const ArrayRCP< const size_t > &NumEntriesPerRowToAlloc, ProfileType pftype=DynamicProfile, const Teuchos::RCP< Teuchos::ParameterList > &params=Teuchos::null);

    //! Constructor specifying column Map and fixed number of entries for each row.
    EpetraCrsMatrixT(const Teuchos::RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &rowMap, const Teuchos::RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &colMap, size_t maxNumEntriesPerRow, ProfileType pftype=DynamicProfile, const Teuchos::RCP< Teuchos::ParameterList > &params=Teuchos::null);

    //! Constructor specifying column Map and number of entries in each row.
    EpetraCrsMatrixT(const Teuchos::RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &rowMap, const Teuchos::RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &colMap, const ArrayRCP< const size_t > &NumEntriesPerRowToAlloc, ProfileType pftype=DynamicProfile, const Teuchos::RCP< Teuchos::ParameterList > &params=Teuchos::null);

    //! Constructor specifying a previously constructed graph.
    EpetraCrsMatrixT(const Teuchos::RCP< const CrsGraph< LocalOrdinal, GlobalOrdinal, Node> > &graph, const Teuchos::RCP< Teuchos::ParameterList > &params=Teuchos::null);


    //! Constructor for a fused import
    EpetraCrsMatrixT(const Teuchos::RCP<const CrsMatrix<Scalar,LocalOrdinal,GlobalOrdinal,Node> >& sourceMatrix,
                    const Import<LocalOrdinal,GlobalOrdinal,Node> &importer,
                    const Teuchos::RCP<const Map<LocalOrdinal,GlobalOrdinal,Node> > & domainMap = Teuchos::null,
                    const Teuchos::RCP<const Map<LocalOrdinal,GlobalOrdinal,Node> > & rangeMap = Teuchos::null,
                    const Teuchos::RCP<Teuchos::ParameterList>& params = Teuchos::null);

    //! Constructor for a fused export
    EpetraCrsMatrixT(const Teuchos::RCP<const CrsMatrix<Scalar,LocalOrdinal,GlobalOrdinal,Node> >& sourceMatrix,
                    const Export<LocalOrdinal,GlobalOrdinal,Node> &exporter,
                    const Teuchos::RCP<const Map<LocalOrdinal,GlobalOrdinal,Node> > & domainMap = Teuchos::null,
                    const Teuchos::RCP<const Map<LocalOrdinal,GlobalOrdinal,Node> > & rangeMap = Teuchos::null,
                    const Teuchos::RCP<Teuchos::ParameterList>& params = Teuchos::null);

    //! Destructor.
    virtual ~EpetraCrsMatrixT() { }

    //@}

    //! @name Insertion/Removal Methods
    //@{

    //! Insert matrix entries, using global IDs.
    void insertGlobalValues(GlobalOrdinal globalRow, const ArrayView< const GlobalOrdinal > &cols, const ArrayView< const Scalar > &vals);

    //! Insert matrix entries, using local IDs.
    void insertLocalValues(LocalOrdinal localRow, const ArrayView< const LocalOrdinal > &cols, const ArrayView< const Scalar > &vals);

    //! Replace matrix entries, using global IDs.
    void replaceGlobalValues(GlobalOrdinal globalRow, const ArrayView< const GlobalOrdinal > &cols, const ArrayView< const Scalar > &vals);

    //! Replace matrix entries, using local IDs.
    void replaceLocalValues(LocalOrdinal localRow, const ArrayView< const LocalOrdinal > &cols, const ArrayView< const Scalar > &vals);

    //! Set all matrix entries equal to scalarThis.
    void setAllToScalar(const Scalar &alpha) { XPETRA_MONITOR("EpetraCrsMatrixT::setAllToScalar"); mtx_->PutScalar(alpha); }

    //! Scale the current values of a matrix, this = alpha*this.
    void scale(const Scalar &alpha) { XPETRA_MONITOR("EpetraCrsMatrixT::scale"); mtx_->Scale(alpha); }

    //! Allocates and returns ArrayRCPs of the Crs arrays --- This is an Xpetra-only routine.
    //** \warning This is an expert-only routine and should not be called from user code. */
    void allocateAllValues(size_t numNonZeros,ArrayRCP<size_t> & rowptr, ArrayRCP<LocalOrdinal> & colind, ArrayRCP<Scalar> & values);

    //! Sets the 1D pointer arrays of the graph.
    void setAllValues(const ArrayRCP<size_t> & rowptr, const ArrayRCP<LocalOrdinal> & colind, const ArrayRCP<Scalar> & values);

    //! Gets the 1D pointer arrays of the graph.
    void getAllValues(ArrayRCP<const size_t>& rowptr, ArrayRCP<const LocalOrdinal>& colind, ArrayRCP<const Scalar>& values) const;

    //! Expert static fill complete
    //** \warning This is an expert-only routine and should not be called from user code. */
    void expertStaticFillComplete(const RCP<const Map<LocalOrdinal,GlobalOrdinal,Node> > & domainMap,
                                  const RCP<const Map<LocalOrdinal,GlobalOrdinal,Node> > & rangeMap,
                                  const RCP<const Import<LocalOrdinal,GlobalOrdinal,Node> > &importer=Teuchos::null,
                                  const RCP<const Export<LocalOrdinal,GlobalOrdinal,Node> > &exporter=Teuchos::null,
                                  const RCP<ParameterList> &params=Teuchos::null);
    //@}

    //! @name Transformational Methods
    //@{

    //!
    void resumeFill(const RCP< ParameterList > &params=null);

    //! Signal that data entry is complete, specifying domain and range maps.
    void fillComplete(const RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &domainMap, const RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &rangeMap, const RCP< ParameterList > &params=null);

    //! Signal that data entry is complete.
    void fillComplete(const RCP< ParameterList > &params=null);


    //!  Replaces the current domainMap and importer with the user-specified objects.
    void replaceDomainMapAndImporter(const Teuchos::RCP< const  Map< LocalOrdinal, GlobalOrdinal, Node > >& newDomainMap, Teuchos::RCP<const Import<LocalOrdinal,GlobalOrdinal,Node> >  & newImporter);
    //@}

    //! @name Methods implementing RowMatrix
    //@{

    //! Returns the communicator.
    const RCP< const Comm< int > >  getComm() const { XPETRA_MONITOR("EpetraCrsMatrixT::getComm"); return toXpetra(mtx_->Comm()); }

    //! Returns the Map that describes the row distribution in this matrix.
    const RCP< const Map<LocalOrdinal, GlobalOrdinal, Node> > getRowMap() const { XPETRA_MONITOR("EpetraCrsMatrixT::getRowMap"); return toXpetra<GlobalOrdinal>(mtx_->RowMap()); }

    //! Returns the Map that describes the column distribution in this matrix.
    const RCP< const Map<LocalOrdinal, GlobalOrdinal, Node> > getColMap() const { XPETRA_MONITOR("EpetraCrsMatrixT::getColMap"); return toXpetra<GlobalOrdinal>(mtx_->ColMap()); }

    //! Returns the CrsGraph associated with this matrix.
    RCP< const CrsGraph<LocalOrdinal, GlobalOrdinal, Node> > getCrsGraph() const { XPETRA_MONITOR("EpetraCrsMatrixT::getCrsGraph"); return toXpetra<GlobalOrdinal>(mtx_->Graph()); }

    //! Number of global elements in the row map of this matrix.
    global_size_t getGlobalNumRows() const { XPETRA_MONITOR("EpetraCrsMatrixT::getGlobalNumRows"); return mtx_->NumGlobalRows64(); }

    //! Number of global columns in the matrix.
    global_size_t getGlobalNumCols() const { XPETRA_MONITOR("EpetraCrsMatrixT::getGlobalNumCols"); return mtx_->NumGlobalCols64(); }

    //! Returns the number of matrix rows owned on the calling node.
    size_t getNodeNumRows() const { XPETRA_MONITOR("EpetraCrsMatrixT::getNodeNumRows"); return mtx_->NumMyRows(); }

    //! Returns the number of columns connected to the locally owned rows of this matrix.
    size_t getNodeNumCols() const { XPETRA_MONITOR("EpetraCrsMatrixT::getNodeNumCols"); return mtx_->NumMyCols(); }

    //! Returns the global number of entries in this matrix.
    global_size_t getGlobalNumEntries() const { XPETRA_MONITOR("EpetraCrsMatrixT::getGlobalNumEntries"); return mtx_->NumGlobalNonzeros64(); }

    //! Returns the local number of entries in this matrix.
    size_t getNodeNumEntries() const { XPETRA_MONITOR("EpetraCrsMatrixT::getNodeNumEntries"); return mtx_->NumMyNonzeros(); }

    //! Returns the current number of entries on this node in the specified local row.
    size_t getNumEntriesInLocalRow(LocalOrdinal localRow) const { XPETRA_MONITOR("EpetraCrsMatrixT::getNumEntriesInLocalRow"); return mtx_->NumMyEntries(localRow); }

    //! Returns the number of global diagonal entries, based on global row/column index comparisons.
    global_size_t getGlobalNumDiags() const { XPETRA_MONITOR("EpetraCrsMatrixT::getGlobalNumDiags"); return mtx_->NumGlobalDiagonals64(); }

    //! Returns the number of local diagonal entries, based on global row/column index comparisons.
    size_t getNodeNumDiags() const { XPETRA_MONITOR("EpetraCrsMatrixT::getNodeNumDiags"); return mtx_->NumMyDiagonals(); }

    //! Returns the maximum number of entries across all rows/columns on all nodes.
    size_t getGlobalMaxNumRowEntries() const { XPETRA_MONITOR("EpetraCrsMatrixT::getGlobalMaxNumRowEntries"); return mtx_->GlobalMaxNumEntries(); }

    //! Returns the maximum number of entries across all rows/columns on this node.
    size_t getNodeMaxNumRowEntries() const { XPETRA_MONITOR("EpetraCrsMatrixT::getNodeMaxNumRowEntries"); return mtx_->MaxNumEntries(); }

    //! If matrix indices are in the local range, this function returns true. Otherwise, this function returns false.
    bool isLocallyIndexed() const { XPETRA_MONITOR("EpetraCrsMatrixT::isLocallyIndexed"); return mtx_->IndicesAreLocal(); }

    //! If matrix indices are in the global range, this function returns true. Otherwise, this function returns false.
    bool isGloballyIndexed() const { XPETRA_MONITOR("EpetraCrsMatrixT::isGloballyIndexed"); return mtx_->IndicesAreGlobal(); }

    //! Returns true if the matrix is in compute mode, i.e. if fillComplete() has been called.
    bool isFillComplete() const;

    //! Returns true if the matrix is in edit mode.
    bool isFillActive() const;

    //! Returns the Frobenius norm of the matrix.
    ScalarTraits< Scalar >::magnitudeType getFrobeniusNorm() const { XPETRA_MONITOR("EpetraCrsMatrixT::getFrobeniusNorm"); return mtx_->NormFrobenius(); }

    //! Returns true if getLocalRowView() and getGlobalRowView() are valid for this class.
    bool supportsRowViews() const;

    //! Extract a list of entries in a specified local row of the matrix. Put into storage allocated by calling routine.
    void getLocalRowCopy(LocalOrdinal LocalRow, const ArrayView< LocalOrdinal > &Indices, const ArrayView< Scalar > &Values, size_t &NumEntries) const;

    //! Extract a list of entries in a specified global row of this matrix. Put into pre-allocated storage.
    void getGlobalRowCopy(GlobalOrdinal GlobalRow, const ArrayView< GlobalOrdinal > &Indices, const ArrayView< Scalar > &Values, size_t &NumEntries) const;

    //! Extract a const, non-persisting view of global indices in a specified row of the matrix.
    void getGlobalRowView(GlobalOrdinal GlobalRow, ArrayView< const GlobalOrdinal > &indices, ArrayView< const Scalar > &values) const;

    //! Extract a const, non-persisting view of local indices in a specified row of the matrix.
    void getLocalRowView(LocalOrdinal LocalRow, ArrayView< const LocalOrdinal > &indices, ArrayView< const Scalar > &values) const;

    //! Get a copy of the diagonal entries owned by this node, with local row indices.
    void getLocalDiagCopy(Vector< Scalar, LocalOrdinal, GlobalOrdinal, Node > &diag) const { XPETRA_MONITOR("EpetraCrsMatrixT::getLocalDiagCopy"); mtx_->ExtractDiagonalCopy(toEpetra(diag)); }

    //! Get offsets of the diagonal entries in the matrix.
    void getLocalDiagOffsets(Teuchos::ArrayRCP<size_t> &offsets) const {
      TEUCHOS_TEST_FOR_EXCEPTION(true, Xpetra::Exceptions::NotImplemented, "Xpetra::EpetraCrsMatrixT.getLocalDiagOffsets() is not implemented or supported.");
    }

    //! Get a copy of the diagonal entries owned by this node, with local row indices, using row offsets.
    void getLocalDiagCopy(Vector< Scalar, LocalOrdinal, GlobalOrdinal, Node > &diag, const Teuchos::ArrayView<const size_t> &offsets) const {
      TEUCHOS_TEST_FOR_EXCEPTION(true, Xpetra::Exceptions::NotImplemented, "Xpetra::EpetraCrsMatrixT.getLocalDiagCopy using offsets is not implemented or supported.");
    }

    //@}

    //! @name Methods implementing Operator
    //@{

    //! Computes the sparse matrix-multivector multiplication.
    void apply(const MultiVector< Scalar, LocalOrdinal, GlobalOrdinal, Node > &X, MultiVector< Scalar, LocalOrdinal, GlobalOrdinal, Node > &Y, Teuchos::ETransp mode=Teuchos::NO_TRANS, Scalar alpha=ScalarTraits< Scalar >::one(), Scalar beta=ScalarTraits< Scalar >::zero()) const;

    //! Returns the Map associated with the domain of this operator. This will be null until fillComplete() is called.
    const RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > >  getDomainMap() const { XPETRA_MONITOR("EpetraCrsMatrixT::getDomainMap"); return toXpetra<GlobalOrdinal>(mtx_->DomainMap()); }

    //!
    const RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > >  getRangeMap() const { XPETRA_MONITOR("EpetraCrsMatrixT::getRangeMap"); return toXpetra<GlobalOrdinal>(mtx_->RangeMap()); }

    //@}

    //! @name Overridden from Teuchos::Describable
    //@{

    //! A simple one-line description of this object.
    std::string description() const;

    //! Print the object with some verbosity level to an FancyOStream object.
    void describe(Teuchos::FancyOStream &out, const Teuchos::EVerbosityLevel verbLevel=Teuchos::Describable::verbLevel_default) const;

    //@}

    //! Deep copy constructor
    EpetraCrsMatrixT(const EpetraCrsMatrixT& matrix);

    //! Implements DistObject interface
    //{@

    //! Access function for the Tpetra::Map this DistObject was constructed with.
    Teuchos::RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > getMap() const { XPETRA_MONITOR("EpetraCrsMatrixT::getMap"); return toXpetra<GlobalOrdinal>(mtx_->Map()); }

    //! Import.
    void doImport(const DistObject<char, LocalOrdinal, GlobalOrdinal, Node> &source, const Import< LocalOrdinal, GlobalOrdinal, Node > &importer, CombineMode CM);

    //! Export.
    void doExport(const DistObject<char, LocalOrdinal, GlobalOrdinal, Node> &dest, const Import< LocalOrdinal, GlobalOrdinal, Node >& importer, CombineMode CM);

    //! Import (using an Exporter).
    void doImport(const DistObject<char, LocalOrdinal, GlobalOrdinal, Node> &source, const Export< LocalOrdinal, GlobalOrdinal, Node >& exporter, CombineMode CM);

    //! Export (using an Importer).
    void doExport(const DistObject<char, LocalOrdinal, GlobalOrdinal, Node> &dest, const Export< LocalOrdinal, GlobalOrdinal, Node >& exporter, CombineMode CM);

    void removeEmptyProcessesInPlace (const Teuchos::RCP<const Map<LocalOrdinal, GlobalOrdinal, Node> >& newMap) { }

    //@}

    //! @name Xpetra specific
    //@{

    //! Does this have an underlying matrix
    bool hasMatrix() const { return !mtx_.is_null();}

    //! EpetraCrsMatrixT constructor to wrap a Epetra_CrsMatrix object
    EpetraCrsMatrixT(const Teuchos::RCP<Epetra_CrsMatrix > &mtx) : mtx_(mtx), isFillResumed_(false) {  }

    //! Get the underlying Epetra matrix
    RCP<const Epetra_CrsMatrix> getEpetra_CrsMatrix() const { return mtx_; }

    //! Get the underlying Epetra matrix
    RCP<Epetra_CrsMatrix> getEpetra_CrsMatrixNonConst() const { return mtx_; } //TODO: remove

   //@}

  private:

    RCP<Epetra_CrsMatrix> mtx_;

    bool isFillResumed_; //< For Epetra, fillResume() is a fictive operation but we need to keep track of it. This boolean is true only is resumeFill() have been called and fillComplete() have not been called afterward.

  }; // EpetraCrsMatrixT class

#ifndef XPETRA_EPETRA_NO_32BIT_GLOBAL_INDICES
  typedef EpetraCrsMatrixT<int> EpetraCrsMatrix;
#endif

#ifndef XPETRA_EPETRA_NO_64BIT_GLOBAL_INDICES
  typedef EpetraCrsMatrixT<long long> EpetraCrsMatrix64;
#endif

} // Xpetra namespace

#define XPETRA_EPETRACRSMATRIX_SHORT
#endif // XPETRA_EPETRACRSMATRIX_HPP
