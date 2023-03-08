#ifndef GAMEPLAY_JUDGE_JUDGERESPONSE
#define GAMEPLAY_JUDGE_JUDGERESPONSE

// Response is how a note respond to the judge request
enum JudgeResponse
{
    ACTIVE,    // Judge again on the next tick
    COMPLETED, // Already judged, no need to ask again
};

#endif /* GAMEPLAY_JUDGE_JUDGERESPONSE */
