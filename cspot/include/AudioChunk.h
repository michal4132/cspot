#ifndef AUDIOCHUNK_H
#define AUDIOCHUNK_H
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include "WrappedMutex.h"
#include "WrappedSemaphore.h"
#include "Crypto.h"
#include "Utils.h"

class AudioChunk {
private:
    /**
     * @brief Calculates a correct IV by performing bignum addition.
     * 
     * @param num Number to add to IV.
     * @return std::vector<uint8_t> 
     */
    std::vector<uint8_t> getIVSum(uint32_t num);

public:
    std::unique_ptr<Crypto> crypto;
    std::vector<uint8_t> decryptedData;
    std::vector<uint8_t> audioKey;
    bool keepInMemory = false;
    WrappedMutex loadingMutex;
    uint32_t startPosition;
    uint32_t endPosition;
    uint16_t seqId;

    size_t headerFileSize = -1;
    bool isLoaded = false;
    bool isFailed = false;

    /**
     * @brief Triggered when audiochunk is fully downloaded and decrypted.
     */
    std::unique_ptr<WrappedSemaphore> isLoadedSemaphore;

    /**
     * @brief 
     */
    std::unique_ptr<WrappedSemaphore> isHeaderFileSizeLoadedSemaphore;


    /**
     * @brief AudioChunk handles all audiochunk related operations.
     * 
     * @param seqId Sequence id of requested chunk
     * @param audioKey Audio key used for decryption of audio data
     * @param startPosition Start position of current chunk in audio file
     * @param predictedEndPosition Predicted end position of given chunk. This is not final positon.
     */
    AudioChunk(uint16_t seqId, std::vector<uint8_t> &audioKey, uint32_t startPosition, uint32_t predictedEndPosition);
    ~AudioChunk();

    /**
     * @brief Appends incoming chunked data to local cache.
     * 
     * @param data encrypted binary audio data.
     */
    void appendData(std::vector<uint8_t> &data);

    /**
     * @brief Performs AES CTR decryption of received data.
     * 
     */
    void decrypt();
};

#endif
