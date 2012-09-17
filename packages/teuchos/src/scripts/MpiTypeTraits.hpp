/// \class MpiTypeTraits
/// \brief Traits class mapping a given \c Packet type to its MPI_Datatype.
/// \tparam Packet The type of data being sent and received.
///
/// \section Teuchos_MpiTypeTraits_Summary Summary
///
/// For a given C++ type Packet, this traits class tells you:
/// 1. The corresponding MPI_Datatype and size (i.e., number of
///    contiguous instances of that MPI_Datatype).  These tell MPI how
///    to send and receive objects of type Packet.
/// 2. Whether the MPI_Datatype is a <i>basic</i> or <i>derived</i>
///    type.  Derived types must be freed after use, by calling
///    MPI_Type_free().  Basic types need not and must not be freed
///    after use.
/// 3. Whether the MPI_Datatype and size are valid for all instances
///    of Packet on an MPI process, at all times in the program, on
///    all MPI processes.  (These are three separate conditions.
///    See the \ref Teuchos_MpiTypeTraits_Three "Three consistency conditions" 
///    section for details.)
///
/// This class is meant mainly for Trilinos packages such as Tpetra
/// and Zoltan2, that use Comm (Teuchos' MPI wrapper) to communicate
/// data.  It is also appropriate for any direct user of Comm.  If you
/// do not use Comm directly, you need not worry about this class.
///
/// This class works by specialization.  If there is no specialization
/// of MpiTypeTraits for a type \c Packet, then expressions involving
/// <tt> MpiTypeTraits<Packet> </tt> will not compile.  We provide
/// specializations for many commonly used C++ types, including all
/// standard built-in integer and floating-point types.  However, you
/// must specialize any C++ Packet types we do not cover.  Be sure to
/// check this header file for the list of specializations that we
/// provide.  Duplicating an existing specialization will result in an
/// error either at compile time or link time.
///
/// \section Teuchos_MpiTypeTraits_Prereq Prerequisites
///
/// In order to understand this documentation, you must first be
/// familiar with MPI (the Message Passing Interface).  In particular,
/// you should be aware that MPI uses an MPI_Datatype and a size to
/// refer to the type and amount of data being sent, received, or
/// accumulated in a communication operation.  You should understand
/// that some MPI_Datatypes ("derived" ones) need to be freed after
/// use by calling MPI_Type_free(), and others ("basic" ones, which
/// come predefined by the MPI standard) need not and must not be
/// freed in this way.
///
/// \section Teuchos_MpiTypeTraits_What What is an MPI_Datatype?
///
/// MPI_Datatype is an <i>opaque handle</i> type.  It is typically
/// implemented either as a pointer to a struct (whose definition you
/// can't see or shouldn't rely upon), or as an integer index into a
/// table.  For more details on how opaque handles and how to manage
/// them, please see the documentation of OpaqueHandle.  We recommend
/// using OpaqueHandle to manage the MPI_Datatype returned by
/// makeType().
///
/// \subsection Teuchos_MpiTypeTraits_What_DontIgnore Ignore MPI_Datatype at your own peril
///
/// Let me say this again: <i>Ignore MPI_Datatype at your own
/// peril</i>.  There are at least three reasons why you should always
/// use the right MPI_Datatype for your data.  Two of them relate to
/// correctness and availability of functionality, and one relates to
/// performance.
///
/// 1. The MPI standard only requires that one-sided communication
///    (e.g., \c MPI_Put, \c MPI_Get, and \c MPI_Accumulate) be
///    defined for basic MPI_Datatype instances.  They need not
///    necessarily work for derived MPI_Datatypes (those which are not
///    built in, which must be freed after use by calling
///    MPI_Type_free()).
/// 2. The MPI standard allows certain reduction operations (e.g.,
///    MPI_Allreduce) to produce incorrect results when using the
///    wrong datatype (e.g., \c MPI_CHAR instead of MPI_DOUBLE for the
///    C++ built-in type \c double), <i>even if</i> the total length
///    of the data is correct.  (This can happen in practice, and in
///    fact was the motivation for redesigning Teuchos' MPI
///    interface.)
/// 3. Using the right MPI_Datatype exposes optimizations to the
///    underlying communication hardware.  For example, reductions
///    with certain basic MPI_Datatypes may be performed in the
///    network hardware, without involving the CPU at all.  This may
///    significantly reduce latency of certain collective operations.
///
/// \subsection Teuchos_MpiTypeTraits_What_NotSer Not just for serialization
///
/// Note that MPI_Datatype is not merely a description of how to
/// serialize an object.  MPI does provide serialization (\c MPI_Pack)
/// and deserialization (\c MPI_Unpack) functions which use
/// MPI_Datatype in this way, but an MPI implementation need not
/// necessarily serialize data in order to send or receive it.  For
/// example, sending an array of \c double between two processes on
/// the same shared-memory node need only copy memory.
///
/// \section Teuchos_MpiTypeTraits_Three Three consistency conditions
///
/// The MPI_Datatype and size corresponding to many built-in C++ \c
/// Packet types, like \c int or \c double, have the pleasant property
/// that they are valid for
/// 1. all <i>instances</i> of Packet on an MPI process,
/// 2. at all <i>times</i> in an execution of the program,
/// 3. on all </i>MPI processes</i>.
///
/// These conditions do not hold for all \c Packet types.  This
/// matters because in order for MPI to send, receive, or perform
/// collectives correctly on objects of type \c Packet, the
/// MPI_Datatypes and sizes must be consistent on all participating
/// processes.  (Note that we didn't say "the same," only
/// "consistent."  It's possible to have different MPI_Datatypes on
/// the sender and receiver; this is useful for doing things like
/// transposing a matrix.  Read an MPI book for more examples.)
///
/// The problem is that the receiving process needs to know how many
/// data there are, and how to receive it.  If the MPI_Datatype and
/// size are the same on both sender and receiver, the sender and
/// receiver need not coordinate, other than to do the actual send or
/// receive.  Otherwise, correct communication requires coordination.
/// For example, if the sizes differ on each end but the MPI_Datatypes
/// do not, then one of the following must take place.  Either
/// - the sending process sends two messages, first the size and then
///   the actual object, or
/// - the sender sends without coordination, the receiving process
///   executes a probe (MPI_Probe() or MPI_Iprobe()) to figure out the
///   size, and then it receives the message.
///
/// The same problem can happen if the MPI_Datatypes are not
/// consistent, even if the sizes are.  Both of these coordination
/// options cost convenience and performance.  Thus, it's important to
/// know in advance whether coordination is necessary.  
///
/// \subsection Teuchos_MpiTypeTraits_Three_Ex Examples violating the consistency conditions
///
/// This section describes several examples of useful \c Packet types
/// which violate one or more of the consistency conditions.
///
/// The C++ Standard Library class <tt>std::string</tt> violates all
/// three consistency conditions.  Different strings may have
/// different lengths (#1), the length of an individual
/// <tt>std::string</tt> instance may change (#2), and therefore of
/// course, different strings may differ in length on different
/// processes (#3).  We see that in general, violation of either #1 or
/// #2 implies violation of #3.
///
/// Each instance of the arbitrary-precision real type provided by the
/// MPFR library may have a different precision.  The precision is set
/// in the MPFR object's constructor.  Therefore, each MPFR instance
/// may have a different size, which violates #1, #2, and therefore
/// #3.  Users could, however, choose to make all MPFR instances the
/// same precision (#1) at all times (#2) on all processes (#3).  This
/// assertion would remove the need for coordination between sending
/// and receiving processes.  The right place to make this assertion
/// is not here, in the traits class, nor is it in Teuchos MPI wrapper
/// Comm or Comm's nonmember helper functions.  It's wherever those
/// MPI wrappers get used to send and receive \c Packet data, for
/// example in Tpetra::Distributor.
///
/// The automatic differentiation types provided by the Sacado package
/// in Trilinos may violate all three conditions, just like MPFR
/// arbitrary-precision types.
///
/// The ARPREC library's arbitrary-precision real type obeys different
/// rules than MPFR.  Each ARPREC instance on a process has the same
/// precision.  However, this precision is a global per-process
/// parameter which may be changed at any time, and which may be
/// different on each MPI process.  This violates Conditions 2 and 3,
/// but not necessarily Condition 1.  Furthermore, users could promise
/// not to change the precision, which would allow the same
/// optimization as mentioned above for MPFR.
///
/// \subsection Teuchos_MpiTypeTraits_Three_NoPath We forbid pathological heterogeneity
///
/// The above discussion excludes certain pathological cases relating
/// to heterogeneous processors or compilers.  For example, our
/// provided specializations of MpiTypeTraits assume that on all MPI
/// processes, for all built-in C++ \c Packet types,
/// <tt>sizeof(Packet)</tt> has the same value.
///
/// It is technically possible to run on clusters with heterogeneous
/// nodes, such that <tt>sizeof(Packet)</tt> for types such as \c
/// bool, \c int, or \c size_t may have different values on different
/// nodes.  Some MPI implementations, such as OpenMPI, even support
/// this case correctly via the XDR back-end communication standard.
/// However, Teuchos and its client packages probably do not work
/// correctly for this use case, so we do not support it here.
///
/// We make no effort to check whether different processes have
/// different values of <tt>sizeof(Packet)</tt> for all built-in C++
/// \c Packet types.  If you find yourself running on a platform with
/// heterogeneous sizes of some built-in types, you will have to
/// rewrite the specializations of MpiTypeTraits for the affected
/// types.  Their \c globallyConsistent and \c globallyConsistentType
/// fields must be \c false, and you must reimplement their packing
/// methods (e.g., pack() and unpack()) to handle this case.  You must
/// also make sure that your MPI implementation can handle it as well.
///
/// \subsection Teuchos_MpiTypeTraits_Three_Use How to use consistency conditions to write generic code
///
/// This traits class tells you enough about which of the above three
/// consistency conditions are true that you may write correct,
/// efficient generic code.  <i>Generic</i> means that you do not have
/// to know what type \c Packet is.  We have distilled the consistency
/// conditions into two Boolean fields:
/// - <tt>globallyConsistent</tt>: If true, then \c Packet satisfies
///   all three consistency conditions.
/// - <tt>globallyConsistentType</tt>: If true, then all
///   <tt>Packet</tt> instances, at all times during execution of a
///   program, on all MPI processes, have a consistent MPI_Datatype.
///   Different \c Packet instances may still have different sizes,
///   depending on whether \c globallyConsistent is true.
///
/// If \c globallyConsistent is true, then the MPI_Datatype and size
/// returned by makeType() may be used to communicate \c Packet
/// instances, without prior coordination of size information.  The
/// sending process need not send the size first in a separate
/// message, and the receiving process need not probe the size.
///
/// If \c globallyConsistentType is true, then MPI processes need only
/// coordinate on the size of \c Packet instances, not on their
/// MPI_Datatypes.  Thus, for sending or receiving a \c Packet array,
/// it suffices to use the MPI_Datatype of the first instance in the
/// array.  In this case, best practice is for the sending process to
/// compute the sizes of all the instances in the array, and send the
/// resulting total size first as a separate message.
///
/// If \c globallyConsistentType is false, then different entries of
/// an array of \c Packet instances may have inconsistent
/// MPI_Datatypes.  Thus you cannot send the array directly using a
/// single MPI_Send call, or receive it using a single MPI_Recv call.
/// A good way to handle this case would be to use MPI_Pack on the
/// sending process to pack each entry in turn into a send buffer of
/// MPI_PACKED, and and MPI_Unpack on the receiving process to unpack
/// the entries.
///
/// \subsection Teuchos_MpiTypeTraits_Three_HowToCoord How to coordinate on the size
///
/// Suppose that you wanted to implement a generic MPI wrapper
/// function for receiving any \c Packet type using a single function
/// call.  For some <tt>Packet</tt> types, it is necessary for the
/// sending and receiving processes to coordinate on the size.  We
/// mentioned two approaches above: sending two messages, or probing.
///
/// The two-message approach is best if our receive wrapper method has
/// blocking receive semantics (e.g., like MPI_Recv).  This is because
/// the receiving process must block anyway on the first message with
/// the size, in order to know the size before starting the second
/// receive.  It is possible, however, to use the two-message approach
/// for a nonblocking receive wrapper.  This can be done by sending
/// the first size message nonblocking, then having the receive
/// wrapper return a callback that keeps the MPI_Request from the
/// first message.  Invoking the callback does the following:
/// 1. Calls MPI_Wait on the size message's MPI_Request.
/// 2. Uses the size to resize the Packet instance, if necessary.
/// 3. Calls a blocking receive (e.g., MPI_Recv) on the second message
///    (with the actual contents).
///
/// This procedure only "pretends" to be nonblocking, because the
/// receiving process does not start receiving the second message
/// until the first has been received.  The probe approach with
/// MPI_Iprobe() allows fully nonblocking receives, but it has the
/// disadvantage that the receive buffer size is not known on input to
/// the \c receive wrapper method.  Here is how the probe approach
/// would work for a fully nonblocking receive on the receiving
/// process:
///
/// 1. Start an MPI_Iprobe to get the message size.
/// 2. The receive method returns a callback.
/// 3. Invoking the callback first calls MPI_Wait on the MPI_Request
///    returned by MPI_Iprobe, then does a blocking receive (e.g.,
///    MPI_Recv) to receive the message.  (Since the MPI_Wait from
///    MPI_Iprobe returned, the message has already arrived.)
///
/// Note that the sending process only needs to know whether to send
/// one or two messages.  A nonblocking send wrapper need only
/// initiate two MPI_Isend operations, the first with the size and the
/// second with the actual data.
///
/// \section Teuchos_MpiTypeTraits_Specialize How to write a specialization
///
/// This header file (Teuchos_MpiTypeTraits.hpp) and the source file
/// Teuchos_MpiTypeTraits.cpp provide many examples of how to
/// specialize this class for specific \c Packet types.  Examples
/// include both built-in (basic) MPI_Datatypes, and derived
/// MPI_Datatypes.  You might find yourself writing a specialization
/// that produces a basic MPI_Datatype if your MPI implementation
/// provides useful basic MPI_Datatypes beyond what the standard
/// guarantees, or if later versions of the MPI standard add new basic
/// MPI_Datatypes.
///
/// The values of needFree, globallyConsistent, and
/// globallyConsistentType in your specialization must be compile-time
/// constants whose values are visible in the header file.  This lets
/// them be used as template parameters.  It also ensures that their
/// values are the same on all MPI processes.
///
/// Here is an example specialization for <tt>std::string</tt>:
/// \code
/// template<>
/// MpiTypeTraits<std::string> {
///   // Don't use MPI_Type_free(); MPI_CHAR is a basic MPI_Datatype.
///   static const bool needFree = false; 
///   // Different strings may have different lengths.
///   static const bool globallyConsistent = false; 
///   // All strings at all times on all MPI processes use MPI_CHAR.
///   static const bool globallyConsistentType = true; 
///   std::pair<MPI_Datatype, size_t> makeType (const std::string& s) {
///     return std::make_pair (MPI_CHAR, s.size ());
///   }
///   void 
///   resizeForReceive (std::string& packet, 
///                     const std::pair<MPI_Datatype, size_t> typeInfo) 
///   {
///     packet.resize (typeInfo.second);
///   }
/// };
/// \endcode
///
/// Note that specializing this class does not make sense for pointer
/// types.  These include "raw" pointers (<tt>T*</tt> for a C++ type
/// \c T), "smart" pointers such as RCP, iterators, and elements of
/// linked data structures with possible cycles (such as vertices in a
/// graph).
///
/// \section Teuchos_MpiTypeTraits_Design Design discussion
///
/// This section is likely only of interest to implementers of
/// specializations of MpiTypeTraits, or to those who have questions
/// about the design choices made by the authors of MpiTypeTraits.
///
/// \subsection Teuchos_MpiTypeTraits_Design_Datatypes Notes on derived datatypes
///
/// The <a
/// href="http://www.mpi-forum.org/docs/mpi22-report/node78.htm#Node78">MPI
/// 2.2 standard</a> says that MPI_Type_free "[m]arks the datatype
/// object associated with datatype for deallocation and sets datatype
/// to MPI_DATATYPE_NULL. Any communication that is currently using
/// this datatype will complete normally.  Freeing a datatype does not
/// affect any other datatype that was built from the freed
/// datatype. The system behaves as if input datatype arguments to
/// derived datatype constructors are passed by value."  We may
/// conclude the following:
/// 1. We may call MPI_Type_free on any MPI_Datatype, even if a
///    nonblocking operation (e.g., an MPI_Irecv) with that datatype
///    hasn't yet completed.
/// 2. We may safely create composite datatypes (e.g.,
///    <tt>std::pair<X,Y></tt>) from previously created derived
///    datatypes.
///
/// The first statement means that we may free an MPI_Datatype any
/// time after using it in an MPI function call.  We need not track
/// MPI_Irecv or any other nonblocking operation (such as the new
/// nonblocking collectives in MPI 3.0).
///
/// The second statement means that a composite MPI_Datatype will work
/// correctly even if its component MPI_Datatypes are freed later.
/// Furthermore, we may free the composite MPI_Datatype either before
/// or after its component MPI_Datatypes are freed.  Thus, we may free
/// an MPI_Datatype any time after using it.  Otherwise, we could
/// never just create an MPI_Datatype; we would have to maintain a
/// table of all MPI_Datatypes we ever needed to create, and manage
/// use of them by wrapping each in an
/// <tt>RCP<OpaqueWrapper<MPI_Datatype> ></tt> with an elaborate tree
/// of references, so that each parent MPI_Datatype persists at least
/// as long as its children.
///
/// \subsection Teuchos_MpiTypeTraits_Design_Raw Why return a "raw" MPI_Datatype?
///
/// Users might wonder why makeType() returns a "raw" MPI_Datatype
/// rather than a "wrapped" <tt>RCP<OpaqueWrapper<MPI_Datatype> ></tt>
/// with a suitable deallocator.  That would be helpful for creating
/// composite datatypes; for example, it would make it easier to write
/// exception-safe code.  We chose to return the "raw" MPI_Datatype
/// first, for the following reasons:
/// 1. Wrapping it adds overhead for the overwhelmingly common case of
///    basic (not derived) datatypes.
/// 2. We want to give clients of this class a choice about their
///    deallocation mechanism.
///
/// Users who want their MPI_Datatypes wrapped in this way may use the
/// nonmember template function makeMpiType().
template<class Packet>
class MpiTypeTraits {
 public:
  //! The type of the data being received and sent.
  typedef Packet packet_type;
  
  /// Whether the MPI_Datatype returned by makeType() is derived or basic.
  /// Derived types must be freed after use with MPI_Type_free(); basic types
  /// must <i>not</i> be freed.  See the
  /// <a href="http://www.mcs.anl.gov/research/projects/mpi/www/www3/MPI_Type_free.html">documentation of MPI_Type_free()</a>.
  ///
  /// \note This is a compile-time constant so that you may use it as
  ///   a template parameter.  It is visible in the header fie so that
  ///   users can be sure that its value is the same on all MPI
  ///   processes.
  static const bool needFree = false;

  /// Whether all Packet instances on all MPI processes have a
  /// consistent MPI_Datatype and size at all times in the program's
  /// execution.
  ///
  /// See the class documentation for a detailed explanation.
  static const bool globallyConsistent = false;

  /// Whether all Packet instances on all MPI processes have a
  /// consistent MPI_Datatype (but possibly different sizes) at all
  /// times in the program's execution.
  ///
  /// See the class documentation for a detailed explanation.
  ///
  /// \\note To Trilinos developers: If implementing a cache for
  ///   MPI_Datatypes, you should not cache MPI_Datatypes
  ///   corresponding to \c Packet types for which this is false.
  static const bool globallyConsistentType = false;

  /// \\brief Return the MPI_Datatype and count corresponding to the Packet type.
  ///
  /// If globallyConsistentType is true, then you may use the returned
  /// MPI_Datatype to send and receive objects of type Packet,
  /// throughout the lifetime of the program.  Otherwise, we make no
  /// guarantees that this is possible; for example, it may be
  /// necessary to agree on the amount of data to send before sending
  /// or receiving it.
  ///
  /// This function requires a \c Packet instance.  In general, for
  /// maximum portability, one may need this in order to compute the
  /// MPI_Datatype, even if \c globallyConsistentType is true.  For
  /// example, one might compute the MPI_Datatype of an
  /// <tt>std::pair<X,Y></tt> instance \c x using MPI_Type_struct, by
  /// measuring address offsets of <tt>x.first</tt> and
  /// <tt>x.second</tt> from the address of \c x itself.  C++ does not
  /// provide a way to perform introspection on the layout of a class
  /// or struct, without actually having an instance of it.  If this
  /// function did not take a \c Packet instance, then \c Packet would
  /// need to be default constructible (so that we could make an
  /// actual \c Packet instance without knowing what its type is).
  /// Furthermore, if \c globallyConsistent is false, then the
  /// returned size depends on the particular instance.
  static std::pair<MPI_Datatype, size_t> makeType (const Packet& example) {
    // Raise a compile-time error in case no specialization of this
    // traits class has been defined for the given Packet type.
    Packet::noSpecializationForThisType();

    // Return a valid value, so the compiler doesn't complain.
    return std::make_pair (MPI_TYPE_NULL, static_cast<size_t> (1));
  }
};

/// \fn makeMpiType
/// \brief Get MPI_Datatype (safely wrapped in OpaqueHandle) and size
///   for a \c Packet instance.
/// \\tparam Packet The type of data being sent and received; same as
///   the \c Packet template parameter of MpiTypeTraits.
/// 
/// Please refer to the documentation of MpiTypeTraits for details.
template<class Packet>
std::pair<RCP<OpaqueHandle<MPI_Datatype> >, size_t>
makeMpiType (const Packet& p)
{
  std::pair<MPI_Datatype, size_t> result = MpiTypeTraits<Packet>::makeType (p);
  RCP<OpaqueHandle<MPI_Datatype> > handle;  
  if (MpiTypeTraits<Packet>::needFree) {
    handle = opaqueHandle (result.first, MPI_Type_free);
  } else { // It's a basic MPI_Datatype; don't call MPI_Type_free after use.
    handle = opaqueHandle (result.first);
  }
  return std::make_pair (handle, result.second);
}
