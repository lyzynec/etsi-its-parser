/* Standard Library Includes */
#include <iostream>

/* PcapPlusPlus Includes */
// TODO

/* RapidJSON Includes */
// TODO

/* Vanetza Includes */
#include <vanetza/net/packet.hpp>
#include <vanetza/asn1/spatem.hpp>
#include <vanetza/asn1/packet_visitor.hpp>

int main() {

    vanetza::ByteBuffer byteBuffer({
                                           0x00, 0x00, 0x48, 0x00, 0x02, 0x00, 0x00, 0x40, // ..H....@
                                           0x00, 0x00, 0x04, 0xe5, 0x48, 0x00, 0x38, 0x00, // ....H.8.
                                           0x02, 0x00, 0x9f, 0x01, 0xf7, 0x59, 0x00, 0x00, // .....Y..
                                           0x10, 0x75, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x01, // .u......
                                           0x54, 0xff, 0x4c, 0xff, 0x3e, 0xff, 0x44, 0xff, // T.L.>.D.
                                           0x03, 0x04, 0x00, 0x00, 0xb1, 0xc6, 0x0c, 0x26, // .......&
                                           0xbd, 0x09, 0x00, 0x00, 0x67, 0x01, 0x0c, 0x17, // ....g...
                                           0x2f, 0x3c, 0xb7, 0x84, 0xd8, 0x00, 0x00, 0x00, // /<......
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
                                           0x88, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, // ........
                                           0xff, 0xff, 0x10, 0xe2, 0xe4, 0x00, 0xc1, 0xea, // ........
                                           0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x10, 0xc7, // ........
                                           0x26, 0x00, 0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00, // &.......
                                           0x89, 0x47, 0x11, 0x00, 0x50, 0x01, 0x20, 0x40, // .G..P. @
                                           0x03, 0x00, 0x01, 0x09, 0x02, 0x00, 0xf5, 0x43, // .......C
                                           0x00, 0x00, 0xbc, 0xcb, 0x42, 0xee, 0x24, 0xda, // ....B.$.
                                           0x9e, 0xd1, 0x24, 0xa5, 0xfe, 0xfd, 0x1d, 0xed, // ..$.....
                                           0x19, 0x5d, 0x09, 0x6d, 0xbb, 0x94, 0x80, 0x00, // .].m....
                                           0x00, 0x00, 0x1d, 0xed, 0x1c, 0x0c, 0x09, 0x6d, // .......m
                                           0xbb, 0x51, 0x01, 0x90, 0x00, 0x00, 0x00, 0x00, // .Q......
                                           0x00, 0x00, 0x07, 0xd4, 0x00, 0x00, 0x02, 0x04, // ........
                                           0x84, 0xda, 0x9e, 0xd1, 0x00, 0x38, 0x52, 0x9f, // .....8R.
                                           0x4e, 0x59, 0x76, 0x65, 0xc7, 0xaf, 0x0a, 0x0f, // NYve....
                                           0x08, 0x2b, 0x20, 0x99, 0xa7, 0x86, 0xbc, 0x38, // .+ ....8
                                           0xf4, 0x58, 0xc6, 0x40, 0x04, 0x4c, 0x01, 0x00, // .X.@.L..
                                           0x27, 0x9e, 0xc8, 0x5f, 0xe0, 0x6a, 0x07, 0x5a, // '.._.j.Z
                                           0x0d, 0xe7, 0xc0, 0x20, 0x87, 0xe4, 0x3f, 0x74, // ... ..?t
                                           0x48, 0xf4, 0x6f, 0xb4, 0x59, 0xb0, 0x40, 0xab, // H.o.Y.@.
                                           0x42, 0x78, 0x04, 0x08, 0x7e, 0x43, 0xf7, 0x44, // Bx..~C.D
                                           0x8f, 0x4e, 0x0d, 0x46, 0x77, 0x04, 0x0e, 0x82, // .N.Fw...
                                           0xb8, 0x70, 0x40, 0xc1, 0x17, 0xc8, 0x80, 0x28, // .p@....(
                                           0x86, 0xa8, 0xbd, 0xa8, 0x9b, 0xa0, 0x81, 0xd0, // ........
                                           0x57, 0x0e, 0x40, 0x20, 0x22, 0xf9, 0x0f, 0x3d, // W.@ "..=
                                           0x10, 0x85, 0x16, 0x75, 0x13, 0x4c, 0x10, 0x1a, // ...u.L..
                                           0xd0, 0xc1, 0x41, 0x17, 0xc8, 0x80, 0x28, 0x86, // ..A...(.
                                           0xa8, 0xbe, 0xe8, 0x99, 0x20, 0x81, 0x53, 0x86, // .... .S.
                                           0xf8, 0x18, 0x11, 0x7c, 0x88, 0x02, 0x88, 0x2e, // ...|....
                                           0x88, 0x2f, 0x19, 0x42, 0x08, 0x1d, 0x05, 0x70, // ./.B...p
                                           0xe1, 0x83, 0x82, 0x1f, 0x90, 0xdf, 0xd1, 0x23, // .......#
                                           0xd1, 0x91, 0xd1, 0x4b, 0x41, 0x02, 0xad, 0x11, // ...KA...
                                           0xf0, 0x40, 0x21, 0xf9, 0x0e, 0x9d, 0x11, 0xed, // .@!.....
                                           0x19, 0x45, 0x14, 0x8c, 0x10, 0x2a, 0xd1, 0x6f, // .E...*.o
                                           0x04, 0x82, 0x1f, 0x90, 0xe7, 0x51, 0x1e, 0xd1, // .....Q..
                                           0x91, 0xd1, 0x4b, 0x41, 0x02, 0xad, 0x19, 0xe0, // ..KA....
                                           0x50, 0x21, 0xf9, 0x0b, 0x05, 0x11, 0xed, 0x38, // P!.....8
                                           0x35, 0x1d, 0xec, 0x10, 0x3a, 0x0a, 0xe1, 0xc4, // 5...:...
                                           0x0b, 0x04, 0x5f, 0x21, 0xec, 0xa2, 0x10, 0xa2, // .._!....
                                           0xab, 0xa2, 0x4b, 0x82, 0x07, 0x41, 0x5c, 0x39, // ..K..A\9
                                           0x21, 0x80, 0x87, 0xe4, 0x2c, 0x14, 0x42, 0xb4, // !...,.B.
                                           0xe0, 0xd4, 0xb2, 0x70, 0x40, 0x6b, 0x47, 0x0d, // ...p@kG.
                                           0x04, 0x5f, 0x22, 0x00, 0xa2, 0x1a, 0xa2, 0x65, // ._"....e
                                           0xa2, 0x32, 0x82, 0x07, 0x41, 0x5c, 0x38, 0xe1, // .2..A\8.
                                           0xc0, 0x87, 0xe4, 0x38, 0x94, 0x48, 0xf4, 0x72, // ...8.H.r
                                           0x34, 0x5c, 0x30, 0xc1, 0xc7, 0x59, 0xa9        // 4\0..Y.
                                   });

    vanetza::CohesivePacket cohesivePacket(byteBuffer, vanetza::OsiLayer::Physical);
    vanetza::UpPacket upPacket(cohesivePacket);

    vanetza::asn1::PacketVisitor<vanetza::asn1::Spatem> visitor;
    std::shared_ptr<const vanetza::asn1::Spatem> spatem = boost::apply_visitor(visitor, upPacket);

    if (spatem == nullptr) {
        std::cout << "bruh" << std::endl;
        return -1;
    }

    std::cout << spatem << std::endl;

    return 0;
}