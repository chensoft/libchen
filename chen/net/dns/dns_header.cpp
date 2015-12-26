/**
 * Created by Jian Chen
 * @since  2015.11.28
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://www.chensoft.com
 */
#include "dns_header.h"

#include <random>
#include <chrono>

using namespace chen;
using namespace chen::dns;

// -----------------------------------------------------------------------------
// header

// filed value
std::int16_t header::id() const
{
    return this->_id;
}

std::uint16_t header::flag() const
{
    return this->_flag;
}

std::uint16_t header::qdcount() const
{
    return this->_qdcount;
}

std::uint16_t header::ancount() const
{
    return this->_ancount;
}

std::uint16_t header::nscount() const
{
    return this->_nscount;
}

std::uint16_t header::arcount() const
{
    return this->_arcount;
}

// flag value
chen::dns::QR header::qr() const
{
    return static_cast<chen::dns::QR>(this->_flag & FLAG_MASK_QR);
}

chen::dns::OPCODE header::opcode() const
{
    return static_cast<chen::dns::OPCODE>(this->_flag & FLAG_MASK_OPCODE);
}

bool header::authoritative() const
{
    return static_cast<bool>(this->_flag & FLAG_MASK_AA);
}

bool header::truncated() const
{
    return static_cast<bool>(this->_flag & FLAG_MASK_TC);
}

bool header::recursionDesired() const
{
    return static_cast<bool>(this->_flag & FLAG_MASK_RD);
}

bool header::recursionAvailable() const
{
    return static_cast<bool>(this->_flag & FLAG_MASK_RA);
}

bool header::zero() const
{
    return static_cast<bool>(this->_flag & FLAG_MASK_Z);
}

bool header::authenticData() const
{
    return static_cast<bool>(this->_flag & FLAG_MASK_AD);
}

bool header::checkingDisabled() const
{
    return static_cast<bool>(this->_flag & FLAG_MASK_CD);
}

chen::dns::RCODE header::rcode() const
{
    return static_cast<chen::dns::RCODE>(this->_flag & FLAG_MASK_RCODE);
}

// set filed value
void header::setId(std::int16_t value)
{
    this->_id = value ? value : header::random();
}

void header::setFlag(std::uint16_t value)
{
    this->_flag = value;
}

void header::setQdcount(std::uint16_t value)
{
    this->_qdcount = value;
}

void header::setAncount(std::uint16_t value)
{
    this->_ancount = value;
}

void header::setNscount(std::uint16_t value)
{
    this->_nscount = value;
}

void header::setArcount(std::uint16_t value)
{
    this->_arcount = value;
}

// set flag value
void header::setQr(chen::dns::QR value)
{
    this->_flag |= (static_cast<std::uint16_t>(value) << FLAG_POS_QR) & FLAG_MASK_QR;
}

void header::setOpcode(chen::dns::OPCODE value)
{
    this->_flag |= (static_cast<std::uint16_t>(value) << FLAG_POS_OPCODE) & FLAG_MASK_OPCODE;
}

void header::setAuthoritative(bool value)
{
    this->_flag |= (static_cast<std::uint16_t>(value) << FLAG_POS_AA) & FLAG_MASK_AA;
}

void header::setTruncated(bool value)
{
    this->_flag |= (static_cast<std::uint16_t>(value) << FLAG_POS_TC) & FLAG_MASK_TC;
}

void header::setRecursionDesired(bool value)
{
    this->_flag |= (static_cast<std::uint16_t>(value) << FLAG_POS_RD) & FLAG_MASK_RD;
}

void header::setRecursionAvailable(bool value)
{
    this->_flag |= (static_cast<std::uint16_t>(value) << FLAG_POS_RA) & FLAG_MASK_RA;
}

void header::setZero(bool value)
{
    this->_flag |= (static_cast<std::uint16_t>(value) << FLAG_POS_Z) & FLAG_MASK_Z;
}

void header::setAuthenticData(bool value)
{
    this->_flag |= (static_cast<std::uint16_t>(value) << FLAG_POS_AD) & FLAG_MASK_AD;
}

void header::setCheckingDisabled(bool value)
{
    this->_flag |= (static_cast<std::uint16_t>(value) << FLAG_POS_CD) & FLAG_MASK_CD;
}

void header::setRcode(chen::dns::RCODE value)
{
    this->_flag |= (static_cast<std::uint16_t>(value) << FLAG_POS_RCODE) & FLAG_MASK_RCODE;
}

// random
std::int16_t header::random()
{
    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_int_distribution<std::int16_t> uniform(1, static_cast<std::int16_t>(0xFFFF));

    auto high = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    return uniform(engine) ^ static_cast<int16_t>(high);
}


// -----------------------------------------------------------------------------
// question

// get filed value
std::string question::qname() const
{
    return this->_qname;
}

chen::dns::RRType question::qtype() const
{
    return this->_qtype;
}

chen::dns::RRClass question::qclass() const
{
    return this->_qclass;
}

// set field value
void question::setQname(const std::string &value)
{
    this->_qname = value;
}

void question::setQtype(chen::dns::RRType value)
{
    this->_qtype = value;
}

void question::setQclass(chen::dns::RRClass value)
{
    this->_qclass = value;
}