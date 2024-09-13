
#ifndef BARCODE_H
#define BARCODE_H

#include<stdio.h>


typedef enum TextAlignment
{
Center=0,
Left=1,
Right=2,
Stretched=3
}TextAlignment;

typedef enum TextPosition
{
UnderBarcode=0,
AboveBarcode=1

}TextPosition;
typedef enum Symbology
	{
        /// <summary>
        /// Code One 2D symbol.
        /// </summary>
        CodeOne = 0,
        /// <summary>
        /// Code 39 (ISO 16388)
        /// </summary>
		Code39,
        /// <summary>
        /// Code 39 extended ASCII.
        /// </summary>
		Code39Extended,
        /// <summary>
        /// Logistics Applications of Automated Marking and Reading Symbol.
        /// </summary>
        LOGMARS,
        /// <summary>
        /// Code 32 (Italian Pharmacode)
        /// </summary>
        Code32,
        /// <summary>
        /// Pharmazentralnummer (PZN - German Pharmaceutical Code)
        /// </summary>
        PharmaZentralNummer,
        /// <summary>
        /// Pharmaceutical Binary Code. 
        /// </summary>
        Pharmacode,
        /// <summary>
        /// Pharmaceutical Binary Code (2 Track)
        /// </summary>
        Pharmacode2Track,
        /// <summary>
        /// Code 93
        /// </summary>
		Code93,
        /// <summary>
        /// Channel Code.
        /// </summary>
        ChannelCode,
        /// <summary>
        /// Telepen Code.
        /// </summary>
        Telepen,
        /// <summary>
        /// Telepen Numeric Code.
        /// </summary>
        TelepenNumeric,
        /// <summary>
        /// Code 128/GS1-128 (ISO 15417)
        /// </summary>
		Code128,
        /// <summary>
        /// European Article Number (14)
        /// </summary>
        EAN14,
        /// <summary>
        /// Serial Shipping Container Code.
        /// </summary>
        SSCC18,
        /// <summary>
        /// Standard 2 of 5 Code.
        /// </summary>
		Standard2of5,
        /// <summary>
        /// Interleaved 2 of 5 Code.
        /// </summary>
		Interleaved2of5,
        /// <summary>
        /// Matrix 2 of 5 Code.
        /// </summary>
        Matrix2of5,
        /// <summary>
        /// IATA 2 of 5 Code.
        /// </summary>
        IATA2of5,
        /// <summary>
        /// Datalogic 2 of 5 Code.
        /// </summary>
        DataLogic2of5,
        /// <summary>
        /// ITF 14 (GS1 2 of 5 Code)
        /// </summary>
		ITF14,
        /// <summary>
        /// Deutsche Post Identcode (DHL)
        /// </summary>
        DeutschePostIdentCode,
        /// <summary>
        /// Deutsche Post Leitcode (DHL)
        /// </summary>
        DeutshePostLeitCode,
        /// <summary>
        /// Codabar Code.
        /// </summary>
		Codabar,
        /// <summary>
        /// MSI Plessey Code.
        /// </summary>
		MSIPlessey,
        /// <summary>
        /// UK Plessey Code.
        /// </summary>
        UKPlessey,
        /// <summary>
        /// Code 11.
        /// </summary>
		Code11,
        /// <summary>
        /// International Standard Book Number.
        /// </summary>
		ISBN,
        /// <summary>
        /// European Article Number (13)
        /// </summary>
		EAN13,
        /// <summary>
        /// European Article Number (8)
        /// </summary>
		EAN8,
        /// <summary>
        /// Universal Product Code (A)
        /// </summary>
		UPCA,
        /// <summary>
        /// Universal Product Code (E)
        /// </summary>
		UPCE,
        /// <summary>
        /// GS1 Databar Omnidirectional.
        /// </summary>
        DatabarOmni,
        /// <summary>
        /// GS1 Databar Omnidirectional Stacked.
        /// </summary>
        DatabarOmniStacked,
        /// <summary>
        /// GS1 Databar Stacked.
        /// </summary>
        DatabarStacked,
        /// <summary>
        /// GS1 Databar Omnidirectional Truncated.
        /// </summary>
        DatabarTruncated,
        /// <summary>
        /// GS1 Databar Limited.
        /// </summary>
        DatabarLimited,
        /// <summary>
        /// GS1 Databar Expanded.
        /// </summary>
        DatabarExpanded,
        /// <summary>
        /// GS1 Databar Expanded Stacked.
        /// </summary>
        DatabarExpandedStacked,
        /// <summary>
        /// Data Matrix (ISO 16022)
        /// </summary>
        DataMatrix,
        /// <summary>
        /// QR Code (ISO 18004)
        /// </summary>
        QRCode,
        /// <summary>
        /// Micro variation of QR Code.
        /// </summary>
        MicroQRCode,
        /// <summary>
        /// Rectangular Micro variation of QR Code.
        /// </summary>
        RectangularMicroQRCode,
        /// <summary>
        /// UPN variation of QR Code.
        /// </summary>
        UPNQR,
        /// <summary>
        /// Aztec (ISO 24778)
        /// </summary>
        Aztec,
        /// <summary>
        /// Aztec Runes.
        /// </summary>
        AztecRunes,
        /// <summary>
        /// Maxicode (ISO 16023)
        /// </summary>
        MaxiCode,
        /// <summary>
        /// PDF417 (ISO 15438)
        /// </summary>
        PDF417,
        /// <summary>
        /// PDF417 Truncated.
        /// </summary>
        PDF417Truncated,
        /// <summary>
        /// Micro PDF417 (ISO 24728)
        /// </summary>
        MicroPDF417,
        /// <summary>
        /// Australia Post Standard.
        /// </summary>
        AusPostStandard,
        /// <summary>
        /// Australia Post Reply Paid.
        /// </summary>
        AusPostReplyPaid,
        /// <summary>
        /// Australia Post Redirect.
        /// </summary>
        AusPostRedirect,
        /// <summary>
        /// Australia Post Routing.
        /// </summary>
        AusPostRouting,
        /// <summary>
        /// United States Postal Service Intelligent Mail.
        /// </summary>
        USPS,
        /// <summary>
        /// PostNET (Postal Numeric Encoding Technique)
        /// </summary>
        PostNet,
        /// <summary>
        /// Planet (Postal Alpha Numeric Encoding Technique)
        /// </summary>
        Planet,
        /// <summary>
        /// Korean Post.
        /// </summary>
        KoreaPost,
        /// <summary>
        /// Facing Identification Mark (FIM)
        /// </summary>
        FIM,
        /// <summary>
        /// UK Royal Mail 4 State Code.
        /// </summary>
        RoyalMail,
        /// <summary>
        /// KIX Dutch 4 State Code.
        /// </summary>
        KixCode,
        /// <summary>
        /// DAFT Code (Generic 4 State Code)
        /// </summary>
        DaftCode,
        /// <summary>
        /// Flattermarken (Markup Code)
        /// </summary>
        Flattermarken,
        /// <summary>
        /// Japanese Post.
        /// </summary>
        JapanPost,
        /// <summary>
        /// Codablock-F 2D symbol.
        /// </summary>
        CodablockF,
        /// <summary>
        /// Code 16K 2D symbol.
        /// </summary>
        Code16K,
        /// <summary>
        /// Dot Code 2D symbol.
        /// </summary>
        DotCode,
        /// <summary>
        /// Grid Matrix 2D symbol.
        /// </summary>
        GridMatrix,
        /// <summary>
        /// Code 49 2D symbol.
        /// </summary>
        Code49,
        /// <summary>
        /// Han Xin 2D symbol.
        /// </summary>
        HanXin,

        /// <summary>
        /// VIN code symbol.
        /// </summary>
        VINCode,

        /// <summary>
        /// Mailmark 4 state postal.
        /// </summary>
        RoyalMailMailmark,

        /// <summary>
        /// Ultracode 2D color symbol.
        /// </summary>
        Ultracode,

        /// <summary>
        /// Not a valid Symbol ID.
        /// </summary>
        Invalid = -1
	}Symbology;

void printBARCODE(char *p);
#endif