import org.jetbrains.kotlinx.lincheck.annotations.Operation
import org.jetbrains.kotlinx.lincheck.annotations.Param
import org.jetbrains.kotlinx.lincheck.check
import org.jetbrains.kotlinx.lincheck.paramgen.IntGen
import org.jetbrains.kotlinx.lincheck.strategy.managed.modelchecking.ModelCheckingOptions
import org.jetbrains.kotlinx.lincheck.strategy.stress.StressCTest
import org.jetbrains.kotlinx.lincheck.strategy.stress.StressOptions
import org.junit.Test
import java.util.SortedSet
import kotlin.collections.ArrayList

@Param(name = "value", gen = IntGen::class, conf = "1:10")
@StressCTest(sequentialSpecification = SortedSet::class)
class SetLincheckTest {
    private val set: Set<Int> = SetImpl()
    @Operation
    fun add(@Param(name = "value") value: Int): Boolean {
        return set.add(value)
    }

    @Operation
    operator fun contains(@Param(name = "value") value: Int): Boolean {
        return set.contains(value)
    }

    @Operation
    fun remove(@Param(name = "value") value: Int): Boolean {
        return set.remove(value)
    }

    @Operation
    fun isEmpty() = set.isEmpty()

    @Operation
    fun iterate(): ArrayList<Int> {
        val list = ArrayList<Int>()
        val iter = set.iterator()
        while (iter.hasNext()) {
            list.add(iter.next())
        }
        return list
    }

    @Test
    fun test() = StressOptions()
                        .actorsBefore(1)
                        .iterations(1000)
                        .actorsAfter(0)
                        .threads(2)
                        .invocationsPerIteration(100)
                        .check(this::class)

    @Test
    fun modelCheckingTest() = ModelCheckingOptions()
        .actorsBefore(1) // To init the HashMap
        .actorsPerThread(2)
        .actorsAfter(0)
        .threads(3) // number of threads in the parallel part
        .iterations(100) // generate 100 random concurrent scenarios
        .invocationsPerIteration(100) // run each generated scenario 1000 times
        .checkObstructionFreedom(true)
        .check(this::class)

//    override fun equals(o: Any?): Boolean {
//        if (this === o) return true
//        if (o == null || javaClass != o.javaClass) return false
//        if (!super.equals(o)) return false
//        val that = o as SetImplTest
//        return set == that.set
//    }
//
//    override fun hashCode(): Int {
//        return Objects.hash(super.hashCode(), set)
//    }

}