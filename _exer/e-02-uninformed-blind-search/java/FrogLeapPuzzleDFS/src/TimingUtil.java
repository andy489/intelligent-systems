import java.lang.reflect.Method;

public class TimingUtil {

    /**
     * Invokes a no-arg method on {@code target}, printing elapsed time.
     * The method must be annotated with {@code @Timed}.
     */
    public static void run(Object target, String methodName) {
        try {
            Method method = target.getClass().getDeclaredMethod(methodName);
            Timed timed = method.getAnnotation(Timed.class);
            String label = (timed != null && !timed.label().isEmpty()) ? timed.label() : methodName;

            long start = System.nanoTime();
            method.invoke(target);
            long elapsedMs = (System.nanoTime() - start) / 1_000_000;

            System.out.printf("[%s] took: %d ms ≈ %d s%n", label, elapsedMs, elapsedMs / 1000);
        } catch (Exception e) {
            throw new RuntimeException("TimingUtil: failed to invoke " + methodName, e);
        }
    }
}
