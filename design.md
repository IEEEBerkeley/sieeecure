## Design
### AES Mode
- **ECB/CBC** \[4\]
	- considered insecure for actual use
	- easiest to implement

### Salt
Unoptimized: TRNG \[1\]  
Optimized: PRNG seeded (and periodically reseeded) with TRNG \[2\] (must reseed at least every 624 PRNG generations)

Paper: "We use a Mersenne Twister pseudorandom number generator (PRNG) to generate cryptographic salts, re-seeded with a true random source every 200 uses. We used existing open-source encryption libraries within our simulation and parameterized gem5 with the above latencies."

Considerations:
- Verilog or C++? (paper uses C++ libraries for simulation)


## Useful Repos and Links
1. [TRNG for salt generation in Verilog](https://github.com/secworks/trng)  
2. [Mersenne Twister (PRNG)](https://github.com/alexforencich/verilog-mersenne)  
3. **[Tiny AES](https://github.com/kokke/tiny-AES-c)** or [General Embedded Cryptography Library (used in paper)](https://github.com/intel/tinycrypt)  
4. [AES in Verilog](https://medium.com/@imgouravsaini/aes-algorithm-and-its-hardware-implementation-on-fpga-a-step-by-step-guide-2bef178db736) + [Some GitHub Repo](https://github.com/michaelehab/AES-Verilog)  
5. [AES Verilog Implementation](https://github.com/secworks/aes), uses [FuseSoC](https://github.com/olofk/fusesoc) package manager though.
6. [Minimal AES Verilog implementation](https://github.com/sumanth-kalluri/128-Bit-AES-Encryption-and-Decryption-in-Verilog)
7. [Another fairly minimal AES Verilog implementaion](https://github.com/nolancon/AES128)
