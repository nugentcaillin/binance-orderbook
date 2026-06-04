#include <gtest/gtest.h>
#include <poll_socket/ring_buffer.hpp>

using namespace poll_socket;


class RingBufferTest : public testing::Test {
protected:
    static constexpr size_t BUF_SIZE = 10;
    static constexpr std::array<char, BUF_SIZE> CONTENT = {
        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'
    };
    static constexpr size_t WRITTEN = 8;
    static constexpr size_t READ = 6;
    static constexpr size_t WRITE_OVERFLOW = 4;
    RingBuffer<BUF_SIZE> new_buf;
    RingBuffer<BUF_SIZE> written_to;
    RingBuffer<BUF_SIZE> written_read;
    RingBuffer<BUF_SIZE> written_overflow;

    RingBufferTest() {
        new_buf = {};
        written_to = {};
        written_read = {};
        written_overflow = {};

        // ensure buffers all have specific content for checking get_pos
        // functions
        memcpy(new_buf.get_write_pos(), CONTENT.data(), BUF_SIZE);
        memcpy(written_to.get_write_pos(), CONTENT.data(), BUF_SIZE);
        memcpy(written_read.get_write_pos(), CONTENT.data(), BUF_SIZE);
        memcpy(written_overflow.get_write_pos(), CONTENT.data(), BUF_SIZE);

        written_to.advance_write_pos(WRITTEN);

        written_read.advance_write_pos(WRITTEN);
        written_read.advance_read_pos(READ);


        // sub then add back one due to write not being allowed
        // to overflow to equal read
        written_overflow.advance_write_pos(BUF_SIZE - 1);
        written_overflow.advance_read_pos(READ);
        written_overflow.advance_write_pos(WRITE_OVERFLOW + 1);


    }
};



/* --- Basic functionality tests --- */
TEST_F(RingBufferTest, NewBufferEmpty) {
    ASSERT_EQ(new_buf.get_space(), BUF_SIZE - 1);
}
TEST_F(RingBufferTest, NewBufferHasNoBytesToRead) {
    ASSERT_EQ(new_buf.get_unread(), 0);
}
TEST_F(RingBufferTest, AdvancingWritePosReducesSpace) {
    ASSERT_EQ(written_to.get_space(), BUF_SIZE - WRITTEN - 1);
}
TEST_F(RingBufferTest, AdvancingWritePosIncreasesBytesToRead) {
    ASSERT_EQ(written_to.get_unread(), WRITTEN);
}
TEST_F(RingBufferTest, AdvancingReadPosReducesBytesToRead) {
    ASSERT_EQ(written_read.get_unread(), WRITTEN - READ);
}
TEST_F(RingBufferTest, AdvancingReadPastWriteThrows) {
    ASSERT_THROW(written_to.advance_read_pos(WRITTEN + 1), std::out_of_range);
}
TEST_F(RingBufferTest, AdvancingReadToEqualWriteDoesntThrow) {
    ASSERT_NO_THROW(written_to.advance_read_pos(WRITTEN));
}

/* --- Wrapping logic tests --- */
TEST_F(RingBufferTest, WriteWrappingRoundToGoPastReadThrows) {
    ASSERT_THROW(written_read.advance_write_pos(BUF_SIZE - WRITTEN + READ + 1), std::out_of_range);
}
TEST_F(RingBufferTest, WriteWrappingRoundToEqualReadThrows) {
    ASSERT_THROW(written_read.advance_write_pos(BUF_SIZE - WRITTEN + READ), std::out_of_range);
}
TEST_F(RingBufferTest, WrappedWriteThrowsIfAttemptToGoPastRead) {
    ASSERT_THROW(written_overflow.advance_write_pos(READ - WRITE_OVERFLOW + 1), std::out_of_range);
}
TEST_F(RingBufferTest, WrappedWriteThrowsIfAttemptToEqualRead) {
    ASSERT_THROW(written_overflow.advance_write_pos(READ - WRITE_OVERFLOW), std::out_of_range);
}
TEST_F(RingBufferTest, ReadWrappingRoundToGoPastWriteThrows) {
    ASSERT_THROW(written_overflow.advance_read_pos(BUF_SIZE - READ + WRITE_OVERFLOW + 1), std::out_of_range);
}
TEST_F(RingBufferTest, ReadWrappingRoundAllowsReadToEqualWriteDoesntThrow) {
    ASSERT_NO_THROW(written_overflow.advance_read_pos(BUF_SIZE - READ + WRITE_OVERFLOW));
}
TEST_F(RingBufferTest, AdvanceReadNonWrappingDoesntThrowWithWrappedWrite) {
    ASSERT_NO_THROW(written_overflow.advance_read_pos(1));
}
TEST_F(RingBufferTest, WriteWrappedRoundMaxWriteSizeBringsWriteJustBeforeRead) {
   ASSERT_EQ(written_overflow.get_space(), READ - WRITE_OVERFLOW - 1);
}
TEST_F(RingBufferTest, WriteWrappedRoundMaxReadSizeReturnsEndOfBuf) {
    ASSERT_EQ(written_overflow.get_unread(), BUF_SIZE - READ);
}
TEST_F(RingBufferTest, NonWriteWrappedSpaceAllowsFullBuffIfReadNotAtZero) {
    ASSERT_EQ(written_read.get_space(), BUF_SIZE - WRITTEN);
}
TEST_F(RingBufferTest, NonWriteWrappedSpaceDoesntAllowFullBuffIfReadAtZero) {
    ASSERT_EQ(written_to.get_space(), BUF_SIZE - WRITTEN - 1);
}
TEST_F(RingBufferTest, NonWriteWrappedMaxReadSizeReturnsSpaceTillWrite) {
    ASSERT_EQ(written_read.get_unread(), WRITTEN - READ);
}

/* --- Pointer correctness tests */
TEST_F(RingBufferTest, NewBufferHasWritePosAtBegin) {
    ASSERT_EQ(*new_buf.get_write_pos(), CONTENT[0]);
}
TEST_F(RingBufferTest, NewBufferHasReadPosAtBegin) {
    ASSERT_EQ(*new_buf.get_read_pos(), CONTENT[0]);
}
TEST_F(RingBufferTest, AdvanceWriteAdvancesWriteCorrectDistance) {
    ASSERT_EQ(*written_to.get_write_pos(), CONTENT[WRITTEN]);
}
TEST_F(RingBufferTest, AdvaceReadAdvancesReadCorrectDistance) {
    ASSERT_EQ(*written_read.get_read_pos(), CONTENT[READ]);
}
TEST_F(RingBufferTest, ReadWrappingRoundWrapsPointerRound) {
    written_overflow.advance_read_pos(BUF_SIZE - READ);
    ASSERT_EQ(*written_overflow.get_read_pos(), CONTENT[0]);
}
TEST_F(RingBufferTest, WriteWrappingRoundWrapsPointerRound) {
    ASSERT_EQ(*written_overflow.get_write_pos(), CONTENT[WRITE_OVERFLOW]);
}

/* --- Misc Errors --- */
TEST_F(RingBufferTest, AttemptingReadsLargerThanBuffThrows) {
    ASSERT_THROW(new_buf.advance_read_pos(BUF_SIZE), std::out_of_range);
}
TEST_F(RingBufferTest, AttemptingWritesLargerThanBuffThrows) {
    ASSERT_THROW(new_buf.advance_write_pos(BUF_SIZE), std::out_of_range);
}
