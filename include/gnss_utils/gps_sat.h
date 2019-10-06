#pragma once

#include "gnss_utils/satellite.h"
#include "gnss_utils/wgs84.h"

#include <stdint.h>
#include <vector>
#include <fstream>


namespace gnss_utils
{

typedef struct {
    int32_t sat; // satellite number
    int32_t iode; // IODE Issue of Data, Ephemeris (ephemeris version)
    int32_t iodc; // IODC Issue of Data, Clock (clock version)
    int32_t sva; // SV accuracy (URA index) IRN-IS-200H p.97
    int32_t svh; // SV health GPS/QZS (0:ok)
    int32_t week; // GPS/QZS: gps week, GAL: galileo week (00=Invalid, 01 = P Code ON, 11 = C/A code ON, 11 = Invalid) // GPS/QZS: code on L2 * GAL/CMP: data sources
    int32_t code;  // GPS/QZS: L2 P data flag (indicates that the NAV data stream was commanded OFF on the P-code of the in-phase component of the L2 channel) *  CMP: nav type
    int32_t flag;
    GTime toe; // Toe
    GTime toc; // clock data reference time (s) (20.3.4.5)
    GTime ttr; // T_trans
    double A; // Semi-Major Axis m
    double e; // Eccentricity (no units)
    double i0; // Inclination Angle at Reference Time (rad)
    double OMG0; // Longitude of Ascending Node of Orbit Plane at Weekly Epoch (rad)
    double omg; // Argument of Perigee (rad)
    double M0; // Mean Anomaly at Reference Time (rad)
    double deln; // Mean Motion Difference From Computed Value (rad)
    double OMGd; // Rate of Right Ascension (rad/s)
    double idot; // Rate of Inclination Angle (rad/s)
    double crc; // Amplitude of the Cosine Harmonic Correction Term to the Orbit Radius
    double crs; // Amplitude of the Sine Harmonic Correction Term to the Orbit Radius (m)
    double cuc; // Amplitude of the Cosine Harmonic Correction Term to the Argument of Latitude (rad)
    double cus; // Amplitude of the Sine Harmonic Correction Term to the Argument of Latitude (rad)
    double cic; // Amplitude of the Cosine Harmonic Correction Term to the Angle of Inclination (rad)
    double cis; // Amplitude of the Sine Harmonic Correction Term to the Angle of Inclination (rad)
    double toes; // Reference Time Ephemeris in week (s)
    double fit; // fit interval (h) (0: 4 hours, 1:greater than 4 hours)
    double f0; // SV clock parameters - af0
    double f1; // SV clock parameters - af1
    double f2; // SV clock parameters - af2 * * GPS/QZS:tgd[0]=TGD (IRN-IS-200H p.103) // group delay parameter * GAL    :tgd[0]=BGD E5a/E1,tgd[1]=BGD E5b/E * CMP    :tgd[0]=BGD1,tgd[1]=BGD
    double tgd[4];
    double Adot; // Adot for CNAV
    double ndot; // ndot for CNAV
} eph_t;


class GPSSat : public Satellite
{
public:
    static const double FREQL1;
    static const double LAMBDA_L1;

    GPSSat(int id, int idx);
    GPSSat(const eph_t& eph, int idx);

    void update(const GTime &time) override;

    void computePVT(const GTime &g,
                    const Eigen::Ref<Eigen::Vector3d> &pos,
                    const Eigen::Ref<Eigen::Vector3d> &vel,
                    const Eigen::Ref<Eigen::Vector2d> &clock) override;

    void computeMeas(const GTime& rec_time,
                     const Eigen::Vector3d& receiver_pos,
                     const Eigen::Vector3d &rec_vel,
                     const Eigen::Vector2d &clk_bias,
                     Eigen::Vector3d &z) override;
    double selectEphemeris(const GTime &time) const override;

    void readFromRawFile(const std::string& filename) override;
    void addEphemeris(const eph_t& eph_);

    int id_;
    int idx_;
    eph_t eph_ = { 0 };
    GTime t;
    Eigen::Vector3d pos;
    Eigen::Vector3d vel;
    Eigen::Vector2d clk;
};



}

