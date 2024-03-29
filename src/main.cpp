#include <iostream>

struct Quark {
    enum Flavor {
        Up, Down, Strange, Charm, Bottom, Top
    } flavor;

    float spin, charge, mass;

    static constexpr Quark Null() { 
        return { .flavor = Quark::Flavor::Up, .spin = 0.0f, .charge = 0.0f, .mass = 0.0f }; 
    } 

    static constexpr Quark Gen1(bool positive_charge, bool positive_spin) {
        if (positive_charge)
            return { .flavor = Quark::Flavor::Up, .spin = positive_spin?0.5f:-0.5f, .charge = 2.0f/3, .mass = 2.2e6 };
        else
            return { .flavor = Quark::Flavor::Down, .spin = positive_spin?0.5f:-0.5f, .charge = -1.0f/3, .mass = 4.7e6 };
    }

    static constexpr Quark Gen2(bool positive_charge, bool positive_spin) {
        if (positive_charge)
            return { .flavor = Quark::Flavor::Charm, .spin = positive_spin?0.5f:-0.5f, .charge = 2.0f/3, .mass = 1.28e9 };
        else
            return { .flavor = Quark::Flavor::Strange, .spin = positive_spin?0.5f:-0.5f, .charge = -1.0f/3, .mass = 9.6e7 };
    }

    static constexpr Quark Gen3(bool positive_charge, bool positive_spin) {
        if (positive_charge)
            return { .flavor = Quark::Flavor::Top, .spin = positive_spin?0.5f:-0.5f, .charge = 2.0f/3, .mass = 1.731e11 };
        else
            return { .flavor = Quark::Flavor::Bottom, .spin = positive_spin?0.5f:-0.5f, .charge = -1.0f/3, .mass = 4.18e9 };
    }
};

struct Baryon {
    Quark q1, q2, q3;

    static constexpr Baryon Null() { 
        return {
            .q1 = Quark::Null(),
            .q2 = Quark::Null(),
            .q3 = Quark::Null()
        }; 
    }

    static constexpr Baryon Proton() {
        return {
            .q1 = Quark::Gen1(true, true),
            .q2 = Quark::Gen1(false, true),
            .q3 = Quark::Gen1(false, false)
        };
    }

    static constexpr Baryon Neutron() {
        return {
            .q1 = Quark::Gen1(true, true),
            .q2 = Quark::Gen1(true, true),
            .q3 = Quark::Gen1(false, false)
        };
    }

    static Baryon Nucleon(int charge) {
        if (charge == 0) return Neutron();
        else return Proton();
    }

    static Baryon Delta(int updown) {
        return {
            .q1 = Quark::Gen1(updown&1 != 0, true),
            .q2 = Quark::Gen1(updown&2 != 0, true),
            .q3 = Quark::Gen2(updown&4 != 0, true)
        };
    }

    static Baryon Hyperon(int strange, bool half_spin, int updown) {
        switch (strange) {
            case 1:
            if (half_spin) return {
                .q1 = Quark::Gen1(updown&1 != 0, true),
                .q2 = Quark::Gen1(updown&2 != 0, true),
                .q3 = Quark::Gen2(false, false)
            };
            else return {
                .q1 = Quark::Gen1(updown&1 != 0, true),
                .q2 = Quark::Gen1(updown&2 != 0, true),
                .q3 = Quark::Gen2(false, true)
            };

            case 2:
            if (half_spin) return {
                .q1 = Quark::Gen1(updown&1 != 0, true),
                .q2 = Quark::Gen2(false, true),
                .q3 = Quark::Gen2(false, false)
            };
            else return {
                .q1 = Quark::Gen1(updown&1 != 0, true),
                .q2 = Quark::Gen2(false, true),
                .q3 = Quark::Gen2(false, true)
            };

            case 3:
            if (half_spin) return Baryon::Null();
            else return {
                .q1 = Quark::Gen2(false, true),
                .q2 = Quark::Gen2(false, true),
                .q3 = Quark::Gen2(false, true)
            };

            default:
            return Baryon::Null();
        }
    }

    static Baryon Lambda(Quark third_quark) {
        /*
        q1    q2    q3    spin
        1     0     0     -1/2
        1     0     1     1/2
        */
        if (third_quark.flavor == Quark::Flavor::Strange || third_quark.flavor == Quark::Flavor::Charm || third_quark.flavor == Quark::Flavor::Bottom) return {
            .q1 = Quark::Gen1(true, true),
            .q2 = Quark::Gen1(false, false),
            .q3 = third_quark
        };
        else return Baryon::Null();
    }

    static Baryon Sigma(Quark third_quark, bool half_spin, int updown) {
        /*
        q1    q2    q3    spin
        1     0     0     -1/2
        1     0     1     1/2
        1     1     1     3/2
        0     0     0     -3/2

        q1 = !q3 NAND !spin = q3 OR spin
        q2 = q3 AND !spin
        */
        if (third_quark.flavor == Quark::Flavor::Strange || third_quark.flavor == Quark::Flavor::Charm || third_quark.flavor == Quark::Flavor::Bottom && half_spin) return {
            .q1 = Quark::Gen1(true, half_spin || third_quark.spin > 0),
            .q2 = Quark::Gen1(false, !half_spin && third_quark.spin > 0),
            .q3 = third_quark
        };
        else return Baryon::Null();
    }
};

int main(int, char**) {
    Baryon a = Baryon::Nucleon(1);
    return 0;
}
