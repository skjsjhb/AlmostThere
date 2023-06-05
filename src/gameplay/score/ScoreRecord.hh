/**
 * @brief Score recorder module.
 *
 * A score recorder should be able to keep tracking of the composition of the
 * current score. To implement this, a score recorder contains a list of records
 * of each score item generated by the game. Each record, or entry, will have
 * its own props to calc the final score.
 *
 * @author skjsjhb
 */

#include "gameplay/objs/NoteDef.hh"
#include "ScoreValue.hh"

#include <map>
#include <memory>

class ScoreRecords;

class ScoreEntry {
public:
  /**
   * @brief Apply changes to the specified score records object.
   *
   * @param sr The records object to apply to.
   */
  virtual void apply(ScoreRecords &sr) const = 0;

  // Destructor
  virtual ~ScoreEntry() = default;

protected:
  /**
   * @brief The time this entry is generated.
   */
  double scTime = 0;

  /**
   * @brief Base score of this entry.
   */
  unsigned int value = 0;
};

class ScoreRecords {
public:
  /**
   * @brief Insert a record.
   *
   * @see `ScoreEntry`
   * @param se The entry object.
   * @return An identifier of this record, for potential necessity of removing
   * it.
   */
  unsigned int addRecord(std::unique_ptr<ScoreEntry> se);

  /**
   * @brief Get the calculated total score.
   * @return The total score.
   */
  unsigned int exportScore();

  /**
   * @brief Remove a record from buffer.
   * @param id The id of the entry to be removed.
   */
  void rmRecord(unsigned int id);

  /**
   * @brief Set the score amplifier.
   *
   * The amplifier will start from 1 and amplify all added score as the value of
   * itself.
   *
   * @param a The amount of changes. e.g. amp = 1, a = 0.2 => amp = 1.2, a =
   * -0.1 => amp = 1.1
   */
  void addAmplifier(double a);

  /**
   * @brief Add a specified amount of score.
   *
   * The score will be amplified by the amplifier and then stored.
   *
   * @param s Score value.
   */
  void addScore(unsigned int s);

protected:
  double amplifier = 1.0;
  unsigned int total = 0;

  // Counter for entry inserting
  unsigned int eid = 1;
  // Indexing the entries
  std::map<unsigned int, std::unique_ptr<ScoreEntry>> entries;
};

class NoteScoreEntry : public ScoreEntry {
public:
  NoteScoreEntry(NoteType n, ScoreGrade g) : nt(n), gd(g) {};

  static std::unique_ptr<NoteScoreEntry> create(NoteType tp, ScoreGrade g) {
    return std::make_unique<NoteScoreEntry>(tp, g);
  };

  void apply(ScoreRecords &sr) const override {
    sr.addScore(NOTE_SCORE_VALUE[nt][gd]);
  };

protected:
  NoteType nt;
  ScoreGrade gd;
};

class ComboScoreEntry : public ScoreEntry {
  // TODO: impl
};

#ifndef GAMEPLAY_SCORE_SCORERECORD
#define GAMEPLAY_SCORE_SCORERECORD

#endif /* GAMEPLAY_SCORE_SCORERECORD */
