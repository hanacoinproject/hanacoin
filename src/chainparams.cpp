// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>

#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    const char* pszTimestamp = "Korea Times 11/Jan/2018 Bitcoin collapses as South Korea threatens to ban trading";
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 1225000;
        consensus.BIP16Height = 0;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("c699ca19d51c98fd39a188570afc6e73237682a57407ea6c7f2d22bdb6ae35e6");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1.1 * 24 * 60 * 60; // 1.1 days
        consensus.nPowTargetSpacing = 1.5 * 60; // 1.5 minutes
        consensus.nPowTargetTimespanDigisheld = 1.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 7560; // 75% of 10080
        consensus.nMinerConfirmationWindow = 10080; // 3.5 days / nPowTargetSpacing * 4 * 0.75
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1488931200; // March 8, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1520467200; // March 8, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1488931200; // March 8, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1520467200; // March 8, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000003b5fe46b5222");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xd1c1bdae051d11b7c3bb1e26bbc473eeecc6dcc2464005f2c9838cda47ebfb9d"); // 302536

        // Hard premine (no difficulty retargeting) 445,000 blocks * 50 = 22,250,000 HanaCoins
        // Soft premine (normal difficulty retargeting) 5,000 blocks * 50 = 250,000 HanaCoins
        consensus.premineBlocks = 445000;

        // Hardfork params
        nSwitchKGWblock = 0;
        nSwitchDIGIblock = 0;
        nSwitchLyra2REv2_DGW = 0;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xae;
        pchMessageStart[1] = 0x2e;
        pchMessageStart[2] = 0x2c;
        pchMessageStart[3] = 0xcb;
        nDefaultPort = 9501;
        nPruneAfterHeight = 100000;
        vAlertPubKey[MAIN_KEY] = ParseHex("04be90e188a1c5d08587b5527f59944d8f8cbc61e2671548e01e4672867a4dcd9f9903e7fbc0bf0a08a959155beafb8ed20422582d3ad5fc6f2405544d64b0a9a5");
        vAlertPubKey[SUB_KEY]  = ParseHex("042c01b8b4cd339b34fff161b1ec819019c5c51e5db70af6f332f5acf183b3dc61a68597022338a01ec37bda9e9f34b6a5e34a97409a907b886df9266133cd35fb");

        genesis = CreateGenesisBlock(1525878567, 263152, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0xc699ca19d51c98fd39a188570afc6e73237682a57407ea6c7f2d22bdb6ae35e6"));
        assert(genesis.hashMerkleRoot == uint256S("0x6cecc2669293484fcb9ac76d1e937b7aee80e49046b67e43bc1d8e403b0498da"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("dnsseed.hanacoin.net");
        vSeeds.emplace_back("dnsseed.hanacoin.org");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 40);  // H
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,55);  // P
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,176);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};
        base58Prefixes[OLD_SECRET_KEY] = std::vector<unsigned char>(1,178);

        bech32_hrp = "mona";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
                { 200,       uint256S("0x1e563e3a2f9035fca366f7cf516fe84ef4405cd2a58af105ffdad4d21c7cbc57") },
                { 50200,     uint256S("0x8985d401fa7933b07eca99e4ce6e715b4bf11877a31f3e047195eccae6fb2be9") },
                { 100400,    uint256S("0x00e0cb4ade846061c35b5807673a925c1364ce6df4f86c139607750c1c9e1c73") },
                { 150600,    uint256S("0xb0810896e6854b4fb179d6214bef94e1ae2f812547ba85b836f44e9b41044556") },
                { 200800,    uint256S("0x2b9dc94a773198398efb0be8fa936c05fb092821d654360b72ff4e877748f8de") },
                { 251000,    uint256S("0x25444bddd38dfa9ed93269db466f9924a0d67ece4d49f2f7a4443433d8ba44e4") },
                { 301200,    uint256S("0xcc98b693f16d1657b3c7b0fb5c49ec1e916a767e73d943ba031b0f45a3845179") },
                { 351400,    uint256S("0x9ee755c14fc1b6aa33640d882c94318cbd0f7629ff87ed76efb1d95218699c4c") },
                { 401600,    uint256S("0x9a4bbebf9166bc3297b0ca02efdafb52278b66267904a3582ea41219c2edb2d0") },
                { 446000,    uint256S("0x69a26d3d13a852542091f514dbd4808ca57f5f2e785a5d19347854d760b57d41") },
            }
        };

        chainTxData = ChainTxData{
            // Data as of block b8f1e3efdbaec9b2adf722a9ad3667c7368e222e044123bfc00dc2054e373bea (height 447457).
            1526267426, // * UNIX timestamp of last known number of transactions
            447458,          // * total number of transactions between genesis and that timestamp
            //   (the tx=... number in the SetBestChain debug.log lines)
            0.025 // * estimated number of transactions per second after that timestamp
        };

        /* disable fallback fee on mainnet */
        m_fallback_fee_enabled = false;
    }
};

/**
 * Testnet (v4)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 1225000;
        consensus.BIP16Height = 0; // always enforce P2SH BIP16 on regtest
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("4df73b297d37d41e66f50780a1ce671c6cafb319fb6676aa5b2af0ea09fd1d0b");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1.1 * 24 * 60 * 60; // 1.1 days
        consensus.nPowTargetSpacing = 1.5 * 60; // 1.5 minutes
        consensus.nPowTargetTimespanDigisheld = 1.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 75; // 75% for testchains
        consensus.nMinerConfirmationWindow = 100; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1488931200; // March 8, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1520467200; // March 8, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1488931200; // March 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1520467200; // March 8, 2018
        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x4df73b297d37d41e66f50780a1ce671c6cafb319fb6676aa5b2af0ea09fd1d0b");

        // No premine in testnet
        consensus.premineBlocks = 0;

        pchMessageStart[0] = 0x91;
        pchMessageStart[1] = 0xdf;
        pchMessageStart[2] = 0x4d;
        pchMessageStart[3] = 0x18;

        // Hardfork params
        nSwitchKGWblock = 0;
        nSwitchDIGIblock = 0;
        nSwitchLyra2REv2_DGW = 0;

        nDefaultPort = 19503;
        nPruneAfterHeight = 1000;
        vAlertPubKey[MAIN_KEY] = ParseHex("04f7f9a86f2895aa62954522365839fdafa53d26bc4466a2b01442b45d0e12c08c87227221a15ff7118450f2ac68a04f31cdc19286d41bbbe28daa1c1b29f0a1f7");
        vAlertPubKey[SUB_KEY]  = ParseHex("04f2a673264160a24a41856a1660a8732856efff389c1f0a93d7ad6eaf4372211bec6d660ad4f2660547d4fb7bba6c9efd2fcd7d75edc089706cb7839e3408c47e");

        genesis = CreateGenesisBlock(1524130420, 62288, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x4df73b297d37d41e66f50780a1ce671c6cafb319fb6676aa5b2af0ea09fd1d0b"));
        assert(genesis.hashMerkleRoot == uint256S("0x6cecc2669293484fcb9ac76d1e937b7aee80e49046b67e43bc1d8e403b0498da"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testnet-dnsseed.hanacoin.net");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,117);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        base58Prefixes[OLD_SECRET_KEY] = std::vector<unsigned char>(1,239);

        bech32_hrp = "tmona";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData){
            {
                { 0, uint256S("0x4df73b297d37d41e66f50780a1ce671c6cafb319fb6676aa5b2af0ea09fd1d0b") },
            }
        };

        chainTxData = ChainTxData{
            1524130420,
            0,
            250};
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Height = 0; // always enforce P2SH BIP16 on regtest
        consensus.BIP34Height = -1; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 33000; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 33000; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1.1 * 24 * 60 * 60; // 1.5 days
        consensus.nPowTargetSpacing = 1.5 * 60; // 1.5 minutes
        consensus.nPowTargetTimespanDigisheld = 1.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        // No premine in regtest
        consensus.premineBlocks = 0;

        // Hardfork params
        nSwitchKGWblock = 0;
        nSwitchDIGIblock = 0;
        nSwitchLyra2REv2_DGW = 0;

        pchMessageStart[0] = 0xf1;
        pchMessageStart[1] = 0xc2;
        pchMessageStart[2] = 0x54;
        pchMessageStart[3] = 0xba;
        nDefaultPort = 20555;
        nPruneAfterHeight = 1000;
        vAlertPubKey[MAIN_KEY] = ParseHex("04f7f9a86f2895aa62954522365839fdafa53d26bc4466a2b01442b45d0e12c08c87227221a15ff7118450f2ac68a04f31cdc19286d41bbbe28daa1c1b29f0a1f7");
        vAlertPubKey[SUB_KEY]  = ParseHex("04f2a673264160a24a41856a1660a8732856efff389c1f0a93d7ad6eaf4372211bec6d660ad4f2660547d4fb7bba6c9efd2fcd7d75edc089706cb7839e3408c47e");

        genesis = CreateGenesisBlock(1296688602, 3, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x719548bfb318084da500e0e4b6fea5d00a79300f2e33ed54a10bc4f477e91710"));
        assert(genesis.hashMerkleRoot == uint256S("0x6cecc2669293484fcb9ac76d1e937b7aee80e49046b67e43bc1d8e403b0498da"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {};

        chainTxData = ChainTxData{
            1296688602,
            0,
            0};

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,117);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        base58Prefixes[OLD_SECRET_KEY] = std::vector<unsigned char>(1,239);

        bech32_hrp = "rmona";

        /* enable fallback fee on regtest */
        m_fallback_fee_enabled = true;
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
