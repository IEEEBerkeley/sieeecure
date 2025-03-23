## Design
### AES Mode
- **ECB/CBC** \[4\]
	- considered insecure for actual use
	- easiest to implement
- GCM
	- industry standard
	- requires dealing with authentication tags (which we probably don't need?)
	- hardest to implement
	- i couldn't find any existing implementations in Verilog
- XTS \[5\]
	- requires two keys,
	- typically used for disk encryption
	- somewhere between ECB/CBC and GCM in terms of difficulty of implementing
- CTR, OFB, CFB
	- stream ciphers so probably no need to consider

### C RegFile
small AES open-source implementation in \[3\] — which one?

### Salt
Unoptimized: TRNG \[1\]  
Optimized: PRNG seeded (and periodically reseeded) with TRNG \[2\] (must reseed at least every 624 PRNG generations)

Paper: "We use a Mersenne Twister pseudorandom number generator (PRNG) to generate cryptographic salts, re-seeded with a true random source every 200 uses. We used existing open-source encryption libraries within our simulation and parameterized gem5 with the above latencies."

Considerations:
- Verilog or C++? (paper uses C++ libraries for simulation)


## Useful Repos and Links
1. [TRNG for salt generation in Verilog](https://github.com/secworks/trng)  
2. [Mersenne Twister (PRNG)](https://github.com/alexforencich/verilog-mersenne)  
3. [Tiny AES](https://github.com/kokke/tiny-AES-c) or [General Embedded Cryptography Library (used in paper)](https://github.com/intel/tinycrypt)  
4. [AES in Verilog](https://medium.com/@imgouravsaini/aes-algorithm-and-its-hardware-implementation-on-fpga-a-step-by-step-guide-2bef178db736) + [Some GitHub Repo](https://github.com/michaelehab/AES-Verilog)  
5. [AES XTS in Verilog](https://github.com/pradyuman/aes-encryption-engine)  