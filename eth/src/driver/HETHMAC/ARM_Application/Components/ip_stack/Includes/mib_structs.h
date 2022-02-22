/****************************************************************************************
*  MIB (Management Information Base) structures
****************************************************************************************/

#ifndef _MIB_H
#define _MIB_H

/****************************************************************************************
*  Includes
****************************************************************************************/

#include <stdint.h>


/****************************************************************************************
*  Definitions
****************************************************************************************/

/* 1.3.6.1.2.1.2.2.1
 * mib-2.interfaces.ifTable.ifEntry
 */
typedef struct MIB_IF_ENTRY_Ttag
{
  const char* pszDescr;          /**< A textual string containing information about the
                                      interface. This string should include the name of
                                      the manufacturer, the product name and the version
                                      of the hardware interface. */
  uint8_t     bType;             /**< The type of interface, distinguished according to
                                      the physical/link protocol(s) immediately 'below'
                                      the network layer in the protocol stack. */
  uint32_t    ulMtu;             /**< The size of the largest datagram which can be
                                      sent/received on the interface, specified in
                                      octets. For interfaces that are used for
                                      transmitting network datagrams, this is the size
                                      of the largest network datagram that can be sent
                                      on the interface. */
  uint32_t    ulSpeed;           /**< An estimate of the interface's current bandwidth
                                      in bits per second. For interfaces which do not
                                      vary in bandwidth or for those where no accurate
                                      estimation can be made, this object should contain
                                      the nominal bandwidth. */
  uint8_t     bPhysAddressLen;   /**< Number of octets of pbPhysAddress */
  uint8_t*    pbPhysAddress;     /**< The interface's address at the protocol layer
                                      immediately 'below' the network layer in the
                                      protocol stack. For interfaces which do not have
                                      such an address (e.g., a serial line), this object
                                      should contain an octet string of zero length. */
  uint8_t     bAdminStatus;      /**< up(1)/down(2)/testing(3)
                                      The desired state of the interface. The
                                      testing(3) state indicates that no operational
                                      packets can be passed. */
  uint8_t     bOperStatus;       /**< up(1)/down(2)/testing(3)
                                      The current operational state of the interface.
                                      The testing(3) state indicates that no operational
                                      packets can be passed. */
  uint64_t    ullLastChange;     /**< The value of sysUpTime at the time the interface
                                      entered its current operational state. If the
                                      current state was entered prior to the last re-
                                      initialization of the local network management
                                      subsystem, then this object contains a zero
                                      value. */
  uint32_t    ulInOctets;        /**< The total number of octets received on the
                                      interface, including framing characters. */
  uint32_t    ulInErrors;        /**< The number of inbound packets that contained
                                      errors preventing them from being deliverable to a
                                      higher-layer protocol. */
  uint32_t    ulInDiscards;      /**< The number of inbound packets which were chosen
                                      to be discarded even though no errors had been
                                      detected to prevent their being deliverable to a
                                      higher-layer protocol. One possible reason for
                                      discarding such a packet could be to free up
                                      buffer space. */
  uint32_t    ulInUcastPkts;     /**< The number of subnetwork-unicast packets delivered to a higher-layer protocol */
  uint32_t    ulInNUcastPkts;    /**< The number of non-unicast (i.e., subnetwork-broadcast or subnetwork-multicast)
                                      packets  delivered to a higher-layer protocol. */
  uint32_t    ulInUnknownProtos; /**< The number of packets received via the interface
                                      which were discarded because of an unknown or unsupported protocol. */
  uint32_t    ulOutOctets;       /**< The total number of octets transmitted out of the interface, including framing characters. */
  uint32_t    ulOutUcastPkts;    /**< The total number of packets that higher-level protocols requested be transmitted to a
                                      subnetwork-unicast address, including those that were discarded or not sent. */
  uint32_t    ulOutNUcastPkts;   /**< The total number of packets that higher-level protocols requested be transmitted to a non-
                                      unicast (i.e., a subnetwork-broadcast or subnetwork-multicast) address, including those
                                      that were discarded or not sent. */
  uint32_t    ulOutErrors;       /**< The number of outbound packets that could not be transmitted because of errors. */
  uint32_t    ulOutDiscards;     /**< The number of outbound packets which were chosen
                                      to be discarded even though no errors had been
                                      detected to prevent their being transmitted. One
                                      possible reason for discarding such a packet could
                                      be to free up buffer space. */
  uint32_t    ulOutQLen;         /**< The length of the output packet queue (in packets). */
  void*       pvSpecific;
} MIB_IF_ENTRY_T;

/* 1.3.6.1.2.1.10.7.2.1
 * mib-2.transmission.dot3.dot3StatsTable.dot3StatsEntry
 */
typedef struct MIB_DOT3_STATS_ENTRY_Ttag
{
  uint32_t    ulAlignmentErrors;           /**< A count of frames received on a particular
                                                interface that are not an integral number of
                                                octets in length and do not pass the FCS check.
                                                The count represented by an instance of this
                                                object is incremented when the alignmentError
                                                status is returned by the MAC service to the
                                                LLC (or other MAC user). Received frames for
                                                which multiple error conditions obtain are,
                                                according to the conventions of IEEE 802.3
                                                Layer Management, counted exclusively according
                                                to the error status presented to the LLC. */
  uint32_t    ulFCSErrors;                 /**< A count of frames received on a particular
                                                interface that are an integral number of octets
                                                in length but do not pass the FCS check.
                                                The count represented by an instance of this
                                                object is incremented when the frameCheckError
                                                status is returned by the MAC service to the
                                                LLC (or other MAC user). Received frames for
                                                which multiple error conditions obtain are,
                                                according to the conventions of IEEE 802.3
                                                Layer Management, counted exclusively according
                                                to the error status presented to the LLC. */
  uint32_t    ulSingleCollisionFrames;     /**< A count of successfully transmitted frames on
                                                a particular interface for which transmission
                                                is inhibited by exactly one collision.
                                                A frame that is counted by an instance of this
                                                object is also counted by the corresponding
                                                instance of either the ifOutUcastPkts,
                                                ifOutMulticastPkts, or ifOutBroadcastPkts,
                                                and is not counted by the corresponding
                                                instance of the dot3StatsMultipleCollisionFrames
                                                object. */
  uint32_t    ulMultipleCollisionFrames;   /**< A count of successfully transmitted frames on
                                                a particular interface for which transmission
                                                is inhibited by more than one collision.
                                                A frame that is counted by an instance of this
                                                object is also counted by the corresponding
                                                instance of either the ifOutUcastPkts,
                                                ifOutMulticastPkts, or ifOutBroadcastPkts,
                                                and is not counted by the corresponding
                                                instance of the dot3StatsSingleCollisionFrames
                                                object. */
  uint32_t    ulSQETestErrors;             /**< A count of times that the SQE TEST ERROR
                                                message is generated by the PLS sublayer for a
                                                particular interface. The SQE TEST ERROR
                                                message is defined in section 7.2.2.2.4 of
                                                ANSI/IEEE 802.3-1985 and its generation is
                                                described in section 7.2.4.6 of the same
                                                document. */
  uint32_t    ulDeferredTransmissions;     /**< A count of frames for which the first
                                                transmission attempt on a particular interface
                                                is delayed because the medium is busy.
                                                The count represented by an instance of this
                                                object does not include frames involved in
                                                collisions. */
  uint32_t    ulLateCollisions;            /**< The number of times that a collision is
                                                detected on a particular interface later than
                                                512 bit-times into the transmission of a
                                                packet.
                                                Five hundred and twelve bit-times corresponds
                                                to 51.2 microseconds on a 10 Mbit/s system. A
                                                (late) collision included in a count
                                                represented by an instance of this object is
                                                also considered as a (generic) collision for
                                                purposes of other collision-related
                                                statistics. */
  uint32_t    ulExcessiveCollisions;       /**< A count of frames for which transmission on a
                                                particular interface fails due to excessive
                                                collisions. */
  uint32_t    ulInternalMacTransmitErrors; /**< A count of frames for which transmission on a
                                                particular interface fails due to an internal
                                                MAC sublayer transmit error. A frame is only
                                                counted by an instance of this object if it is
                                                not counted by the corresponding instance of
                                                either the dot3StatsLateCollisions object, the
                                                dot3StatsExcessiveCollisions object, or the
                                                dot3StatsCarrierSenseErrors object.
                                                The precise meaning of the count represented by
                                                an instance of this object is implementation-
                                                specific. In particular, an instance of this
                                                object may represent a count of transmission
                                                errors on a particular interface that are not
                                                otherwise counted. */
  uint32_t    ulCarrierSenseErrors;        /**< The number of times that the carrier sense
                                                condition was lost or never asserted when
                                                attempting to transmit a frame on a particular
                                                interface.
                                                The count represented by an instance of this
                                                object is incremented at most once per
                                                transmission attempt, even if the carrier sense
                                                condition fluctuates during a transmission
                                                attempt. */
  uint32_t    ulFrameTooLongs;             /**< A count of frames received on a particular
                                                interface that exceed the maximum permitted
                                                frame size.
                                                The count represented by an instance of this
                                                object is incremented when the frameTooLong
                                                status is returned by the MAC service to the
                                                LLC (or other MAC user). Received frames for
                                                which multiple error conditions obtain are,
                                                according to the conventions of IEEE 802.3
                                                Layer Management, counted exclusively according
                                                to the error status presented to the LLC. */
  uint32_t    ulInternalMacReceiveErrors;  /**< A count of frames for which reception on a
                                                particular interface fails due to an internal
                                                MAC sublayer receive error. A frame is only
                                                counted by an instance of this object if it is
                                                not counted by the corresponding instance of
                                                either the dot3StatsFrameTooLongs object, the
                                                dot3StatsAlignmentErrors object, or the
                                                dot3StatsFCSErrors object.
                                                The precise meaning of the count represented by
                                                an instance of this object is implementation-
                                                specific. In particular, an instance of this
                                                object may represent a count of receive errors
                                                on a particular interface that are not
                                                otherwise counted. */
} MIB_DOT3_STATS_ENTRY_T;

#endif /* _MIB_H */
